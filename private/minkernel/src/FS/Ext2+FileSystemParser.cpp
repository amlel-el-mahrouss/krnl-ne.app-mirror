// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

// Read-only EXT2/EXT3 parser. Write support is intentionally disabled.
// See notes at the bottom of this file.

#ifndef __NE_MINIMAL_OS__
#ifdef __FSKIT_INCLUDES_EXT2__

#include <FSKit/Ext2+IFS.h>
#include <FSKit/Ext2.h>
#include <KernelKit/DebugOutput.h>
#include <KernelKit/FileMgr.h>
#include <KernelKit/HeapMgr.h>
#include <NeKit/ErrorOr.h>
#include <NeKit/KString.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/Utils.h>

namespace Ne::Kernel {

namespace {

// Returns rec_len on success, 0 if the entry is malformed or crosses the
// block boundary. 0 means "stop scanning this block."
UInt16 ext2_dir_entry_ok(const UInt8* block, UInt32 offset, UInt32 blockSize) {
  if (offset + kExt2FSDirEntryHeaderLen > blockSize) return 0;
  auto h = reinterpret_cast<const EXT2_DIR_ENTRY_HEADER*>(block + offset);
  const UInt16 rec = h->fRecordLength;
  if (rec < kExt2FSDirEntryHeaderLen)     return 0;
  if (rec & 0x3u)                         return 0;
  if (offset + rec > blockSize)           return 0;
  if (kExt2FSDirEntryHeaderLen + h->fNameLength > rec) return 0;
  return rec;
}

// Split path into canonicalised components by NUL-terminating in-place.
// Returns component count, or -1 if path is too deep / allocation failed.
// On success, *outBuf and *outComps must be mm_free_ptr'd by caller.
Int32 ext2_split_path(const Char* path, Char** outBuf, const Char*** outComps) {
  constexpr UInt32 kMax = 64;

  *outBuf   = nullptr;
  *outComps = nullptr;

  const SizeT len = rt_string_len(path);
  if (len == 0) return 0;

  Char* buf = static_cast<Char*>(mm_alloc_ptr(len + 1, Yes, No));
  if (!buf) return -1;
  rt_copy_memory_safe(const_cast<Char*>(path), buf, len, len + 1);
  buf[len] = '\0';

  const Char** comps = static_cast<const Char**>(
      mm_alloc_ptr(sizeof(Char*) * kMax, Yes, No));
  if (!comps) {
    mm_free_ptr(buf);
    return -1;
  }

  UInt32 n = 0;
  Char*  p = buf;
  while (*p) {
    while (*p == '/') ++p;
    if (!*p) break;
    Char* start = p;
    while (*p && *p != '/') ++p;
    if (*p) *p++ = '\0';

    const SizeT clen = rt_string_len(start);
    if (clen == 1 && start[0] == '.') continue;
    if (clen == 2 && start[0] == '.' && start[1] == '.') {
      if (n > 0) --n;
      continue;
    }
    if (n >= kMax) {
      mm_free_ptr(comps);
      mm_free_ptr(buf);
      return -1;
    }
    comps[n++] = start;
  }

  *outBuf   = buf;
  *outComps = comps;
  return static_cast<Int32>(n);
}

// phys == 0 && valid == YES means "zero-fill".
struct block_lookup {
  UInt32 phys;
  BOOL   valid;
};

ErrorOr<block_lookup> ext2_lookup_block(Ext2Context* c, const Ext2Node* n,
                                        UInt64 logical) {
  const SizeT  bs  = ext2_block_size(c);
  const UInt64 ppb = bs / sizeof(UInt32);

  if (logical < kExt2FSDirectBlocks) {
    return ErrorOr<block_lookup>(block_lookup{n->fInode.fBlock[logical], YES});
  }

  const UInt64 base1 = kExt2FSDirectBlocks;
  const UInt64 end1  = base1 + ppb;
  if (logical < end1) {
    const UInt32 ind = n->fInode.fBlock[kExt2FSSingleIndirect];
    if (ind == 0) return ErrorOr<block_lookup>(block_lookup{0, YES});

    UInt8* buf = ext2_cache_get(c, ind);
    if (!buf) return ErrorOr<block_lookup>(kErrorDisk);
    const UInt32 p = reinterpret_cast<UInt32*>(buf)[logical - base1];
    ext2_cache_put(c, buf);
    return ErrorOr<block_lookup>(block_lookup{p, YES});
  }

  const UInt64 base2 = end1;
  const UInt64 end2  = base2 + ppb * ppb;
  if (logical < end2) {
    const UInt32 d = n->fInode.fBlock[kExt2FSDoubleIndirect];
    if (d == 0) return ErrorOr<block_lookup>(block_lookup{0, YES});

    UInt8* buf = ext2_cache_get(c, d);
    if (!buf) return ErrorOr<block_lookup>(kErrorDisk);
    const UInt64 w   = logical - base2;
    const UInt32 i1  = static_cast<UInt32>(w / ppb);
    const UInt32 i2  = static_cast<UInt32>(w % ppb);
    const UInt32 ind = reinterpret_cast<UInt32*>(buf)[i1];
    ext2_cache_put(c, buf);
    if (ind == 0) return ErrorOr<block_lookup>(block_lookup{0, YES});

    buf = ext2_cache_get(c, ind);
    if (!buf) return ErrorOr<block_lookup>(kErrorDisk);
    const UInt32 p = reinterpret_cast<UInt32*>(buf)[i2];
    ext2_cache_put(c, buf);
    return ErrorOr<block_lookup>(block_lookup{p, YES});
  }

  const UInt64 base3 = end2;
  const UInt64 end3  = base3 + ppb * ppb * ppb;
  if (logical < end3) {
    const UInt32 t = n->fInode.fBlock[kExt2FSTripleIndirect];
    if (t == 0) return ErrorOr<block_lookup>(block_lookup{0, YES});

    UInt8* buf = ext2_cache_get(c, t);
    if (!buf) return ErrorOr<block_lookup>(kErrorDisk);
    const UInt64 w    = logical - base3;
    const UInt64 pd   = ppb * ppb;
    const UInt32 i1   = static_cast<UInt32>(w / pd);
    const UInt64 tail = w % pd;
    const UInt32 i2   = static_cast<UInt32>(tail / ppb);
    const UInt32 i3   = static_cast<UInt32>(tail % ppb);

    const UInt32 dblk = reinterpret_cast<UInt32*>(buf)[i1];
    ext2_cache_put(c, buf);
    if (dblk == 0) return ErrorOr<block_lookup>(block_lookup{0, YES});

    buf = ext2_cache_get(c, dblk);
    if (!buf) return ErrorOr<block_lookup>(kErrorDisk);
    const UInt32 ind = reinterpret_cast<UInt32*>(buf)[i2];
    ext2_cache_put(c, buf);
    if (ind == 0) return ErrorOr<block_lookup>(block_lookup{0, YES});

    buf = ext2_cache_get(c, ind);
    if (!buf) return ErrorOr<block_lookup>(kErrorDisk);
    const UInt32 p = reinterpret_cast<UInt32*>(buf)[i3];
    ext2_cache_put(c, buf);
    return ErrorOr<block_lookup>(block_lookup{p, YES});
  }

  return ErrorOr<block_lookup>(kErrorDiskIsCorrupted);
}

ErrorOr<UInt32> ext2_dir_lookup(Ext2Context* c, const Ext2Node* dir,
                                 const Char* name, SizeT nameLen) {
  if (!name || nameLen == 0 || nameLen > kExt2FSMaxFileNameLen)
    return ErrorOr<UInt32>(kErrorInvalidData);
  if (ext2_inode_type(dir->fInode) != kExt2FileTypeDirectory)
    return ErrorOr<UInt32>(kErrorDirectoryNotFound);

  const SizeT  bs   = ext2_block_size(c);
  const UInt64 size = ext2_inode_size(dir->fInode,
                                      c->fSuperblock->fFeatureROCompat);
  const UInt64 nblk = (size + bs - 1) / bs;

  for (UInt64 l = 0; l < nblk; ++l) {
    auto lk = ext2_lookup_block(c, dir, l);
    if (lk.HasError()) return ErrorOr<UInt32>(lk.Error());
    const block_lookup bl = lk.Leak().Leak();
    if (bl.phys == 0) continue;

    UInt8* buf = ext2_cache_get(c, bl.phys);
    if (!buf) return ErrorOr<UInt32>(kErrorDisk);

    UInt32 ofs = 0;
    while (ofs < bs) {
      const UInt16 rec = ext2_dir_entry_ok(buf, ofs, bs);
      if (rec == 0) break;
      auto h = reinterpret_cast<const EXT2_DIR_ENTRY_HEADER*>(buf + ofs);
      if (h->fInode != 0 && h->fNameLength == nameLen) {
        const Char* entryName = reinterpret_cast<const Char*>(
            buf + ofs + kExt2FSDirEntryHeaderLen);
        if (rt_string_cmp(name, entryName, nameLen) == 0) {
          const UInt32 ino = h->fInode;
          ext2_cache_put(c, buf);
          return ErrorOr<UInt32>(ino);
        }
      }
      ofs += rec;
    }
    ext2_cache_put(c, buf);
  }

  return ErrorOr<UInt32>(kErrorFileNotFound);
}

}  // namespace

Ext2FileSystemParser::Ext2FileSystemParser(DriveTrait* drive)
    : fCtx(new Ext2Context(drive)) {
  MUST_PASS(fCtx);
  auto r = ext2_mount(fCtx);
  if (r.HasError()) kout << "ext2: mount failed\r";
}

Ext2FileSystemParser::~Ext2FileSystemParser() {
  if (fCtx) {
    delete fCtx;
    fCtx = nullptr;
  }
}

VoidPtr Ext2FileSystemParser::Open(const Char* path, const Char* restrict_type) {
  NE_UNUSED(restrict_type);

  if (!fCtx || !fCtx->fSuperblock) return nullptr;
  if (!path || !*path) return nullptr;

  // Scratch path buffer for symlink splicing. Capped at PATH_MAX.
  constexpr SizeT kPathMax    = 4096;
  constexpr UInt32 kSymLoopMax = 40;

  Char* pathbuf = static_cast<Char*>(mm_alloc_ptr(kPathMax, Yes, No));
  if (!pathbuf) return nullptr;

  const SizeT inLen = rt_string_len(path);
  if (inLen >= kPathMax) {
    mm_free_ptr(pathbuf);
    err_local_get() = kErrorInvalidData;
    return nullptr;
  }
  rt_copy_memory_safe(const_cast<Char*>(path), pathbuf, inLen, kPathMax);
  pathbuf[inLen] = '\0';

  // Scratch target buffer for symlink content.
  Char* linkbuf = static_cast<Char*>(mm_alloc_ptr(kPathMax, Yes, No));
  if (!linkbuf) {
    mm_free_ptr(pathbuf);
    return nullptr;
  }

  UInt32 loops = 0;

  while (YES) {
    // "/" early-out: root directory.
    if (pathbuf[0] == '/' && pathbuf[1] == '\0') {
      mm_free_ptr(linkbuf);
      mm_free_ptr(pathbuf);
      auto r = ext2_load_inode(fCtx, kExt2FSRootInodeNumber);
      if (r.HasError()) return nullptr;
      return reinterpret_cast<VoidPtr>(r.Leak().Leak());
    }

    Char*        tmpBuf   = nullptr;
    const Char** comps    = nullptr;
    const Int32  n        = ext2_split_path(pathbuf, &tmpBuf, &comps);
    if (n < 0) {
      mm_free_ptr(linkbuf);
      mm_free_ptr(pathbuf);
      return nullptr;
    }
    if (n == 0) {
      if (tmpBuf) mm_free_ptr(tmpBuf);
      if (comps)  mm_free_ptr(comps);
      mm_free_ptr(linkbuf);
      mm_free_ptr(pathbuf);
      return nullptr;
    }

    UInt32 ino = kExt2FSRootInodeNumber;

    BOOL      restart   = NO;
    Ext2Node* finalNode = nullptr;

    for (Int32 i = 0; i < n; ++i) {
      auto r = ext2_load_inode(fCtx, ino);
      if (r.HasError()) {
        mm_free_ptr(comps);
        mm_free_ptr(tmpBuf);
        mm_free_ptr(linkbuf);
        mm_free_ptr(pathbuf);
        return nullptr;
      }
      Ext2Node* parent = r.Leak().Leak();

      if (ext2_inode_type(parent->fInode) != kExt2FileTypeDirectory) {
        mm_free_ptr(parent);
        mm_free_ptr(comps);
        mm_free_ptr(tmpBuf);
        mm_free_ptr(linkbuf);
        mm_free_ptr(pathbuf);
        return nullptr;
      }

      auto lk = ext2_dir_lookup(fCtx, parent, comps[i], rt_string_len(comps[i]));
      if (lk.HasError()) {
        mm_free_ptr(parent);
        mm_free_ptr(comps);
        mm_free_ptr(tmpBuf);
        mm_free_ptr(linkbuf);
        mm_free_ptr(pathbuf);
        return nullptr;
      }
      const UInt32 childIno = lk.Leak().Leak();
      if (childIno == 0) {
        mm_free_ptr(parent);
        mm_free_ptr(comps);
        mm_free_ptr(tmpBuf);
        mm_free_ptr(linkbuf);
        mm_free_ptr(pathbuf);
        return nullptr;
      }

      auto cr = ext2_load_inode(fCtx, childIno);
      if (cr.HasError()) {
        mm_free_ptr(parent);
        mm_free_ptr(comps);
        mm_free_ptr(tmpBuf);
        mm_free_ptr(linkbuf);
        mm_free_ptr(pathbuf);
        return nullptr;
      }
      Ext2Node* child = cr.Leak().Leak();

      // Follow symlink (whether or not it's the final component).
      if (ext2_inode_type(child->fInode) == kExt2FileTypeSymbolicLink) {
        if (++loops > kSymLoopMax) {
          mm_free_ptr(child);
          mm_free_ptr(parent);
          mm_free_ptr(comps);
          mm_free_ptr(tmpBuf);
          mm_free_ptr(linkbuf);
          mm_free_ptr(pathbuf);
          err_local_get() = kErrorTimeout;
          return nullptr;
        }

        const Int32 llen = this->ReadLink(child, linkbuf, kPathMax);
        mm_free_ptr(child);
        child = nullptr;

        if (llen < 0) {
          mm_free_ptr(parent);
          mm_free_ptr(comps);
          mm_free_ptr(tmpBuf);
          mm_free_ptr(linkbuf);
          mm_free_ptr(pathbuf);
          return nullptr;
        }

        // newpath = linkbuf + "/" + comps[i+1..n-1].
        // comps[] are NUL-separated spans into tmpBuf; reassemble them.
        SizeT pos = 0;
        if (linkbuf[0] == '/') {
          rt_copy_memory_safe(linkbuf, pathbuf,
                              static_cast<SizeT>(llen), kPathMax);
          pos = static_cast<SizeT>(llen);
        } else {
          // Relative target resolves against the parent dir. Rebuild the
          // path-to-parent from canonicalised comps[0..i-1].
          pathbuf[0] = '/';
          pos = 1;
          for (Int32 k = 0; k < i; ++k) {
            const SizeT cl = rt_string_len(comps[k]);
            if (pos + cl + 1 >= kPathMax) goto overflow;
            rt_copy_memory_safe(const_cast<Char*>(comps[k]), pathbuf + pos,
                                cl, kPathMax - pos);
            pos += cl;
            pathbuf[pos++] = '/';
          }
          if (pos + static_cast<SizeT>(llen) >= kPathMax) goto overflow;
          rt_copy_memory_safe(linkbuf, pathbuf + pos,
                              static_cast<SizeT>(llen), kPathMax - pos);
          pos += static_cast<SizeT>(llen);
        }

        for (Int32 k = i + 1; k < n; ++k) {
          const SizeT cl = rt_string_len(comps[k]);
          if (pos + cl + 1 >= kPathMax) goto overflow;
          pathbuf[pos++] = '/';
          rt_copy_memory_safe(const_cast<Char*>(comps[k]), pathbuf + pos,
                              cl, kPathMax - pos);
          pos += cl;
        }
        pathbuf[pos] = '\0';

        mm_free_ptr(parent);
        mm_free_ptr(comps);
        mm_free_ptr(tmpBuf);
        restart = YES;
        break;

      overflow:
        mm_free_ptr(parent);
        mm_free_ptr(comps);
        mm_free_ptr(tmpBuf);
        mm_free_ptr(linkbuf);
        mm_free_ptr(pathbuf);
        err_local_get() = kErrorInvalidData;
        return nullptr;
      }

      mm_free_ptr(parent);

      // Advance.
      if (i == n - 1) {
        finalNode = child;
        ino = childIno;
      } else {
        if (ext2_inode_type(child->fInode) != kExt2FileTypeDirectory) {
          mm_free_ptr(child);
          mm_free_ptr(comps);
          mm_free_ptr(tmpBuf);
          mm_free_ptr(linkbuf);
          mm_free_ptr(pathbuf);
          return nullptr;
        }
        mm_free_ptr(child);
        ino = childIno;
      }
    }

    if (restart) continue;

    mm_free_ptr(comps);
    mm_free_ptr(tmpBuf);
    mm_free_ptr(linkbuf);
    mm_free_ptr(pathbuf);
    return reinterpret_cast<VoidPtr>(finalNode);
  }
}

VoidPtr Ext2FileSystemParser::Read(VoidPtr node, Int32 flags, SizeT size) {
  NE_UNUSED(flags);

  if (!node || size == 0 || !fCtx || !fCtx->fSuperblock) return nullptr;

  auto n = reinterpret_cast<Ext2Node*>(node);
  if (ext2_inode_type(n->fInode) == kExt2FileTypeDirectory) {
    err_local_get() = kErrorInvalidData;
    return nullptr;
  }

  const UInt64 fsz = ext2_inode_size(n->fInode, fCtx->fSuperblock->fFeatureROCompat);
  if (n->fCursor >= fsz) return nullptr;

  const UInt64 avail = fsz - n->fCursor;
  const SizeT  want  = static_cast<SizeT>(avail < size ? avail : size);
  if (want == 0) return nullptr;

  UInt8* out = static_cast<UInt8*>(mm_alloc_ptr(want, Yes, No));
  if (!out) {
    err_local_get() = kErrorHeapOutOfMemory;
    return nullptr;
  }

  const SizeT bs = ext2_block_size(fCtx);

  UInt8* dst       = out;
  SizeT  remaining = want;
  UInt64 off       = n->fCursor;

  while (remaining > 0) {
    const UInt64 logical = off / bs;
    const UInt32 ofsInBlk = static_cast<UInt32>(off % bs);
    const UInt32 chunk    = static_cast<UInt32>(
        remaining < bs - ofsInBlk ? remaining : bs - ofsInBlk);

    auto lk = ext2_lookup_block(fCtx, n, logical);
    if (lk.HasError()) {
      mm_free_ptr(out);
      err_local_get() = lk.Error();
      return nullptr;
    }
    const block_lookup bl = lk.Leak().Leak();

    if (bl.phys == 0) {
      rt_zero_memory(dst, chunk);
    } else {
      UInt8* blk = ext2_cache_get(fCtx, bl.phys);
      if (!blk) {
        mm_free_ptr(out);
        err_local_get() = kErrorDisk;
        return nullptr;
      }
      rt_copy_memory_safe(blk + ofsInBlk, dst, chunk, chunk);
      ext2_cache_put(fCtx, blk);
    }

    dst       += chunk;
    off       += chunk;
    remaining -= chunk;
  }

  n->fCursor += want;
  return out;
}

BOOL Ext2FileSystemParser::Seek(VoidPtr node, SizeT offset) {
  if (!node || !fCtx || !fCtx->fSuperblock) return NO;
  auto n = reinterpret_cast<Ext2Node*>(node);
  const UInt64 fsz = ext2_inode_size(n->fInode, fCtx->fSuperblock->fFeatureROCompat);
  if (offset > fsz) return NO;
  n->fCursor = offset;
  return YES;
}

SizeT Ext2FileSystemParser::Tell(VoidPtr node) {
  if (!node) return 0;
  return static_cast<SizeT>(reinterpret_cast<Ext2Node*>(node)->fCursor);
}

BOOL Ext2FileSystemParser::Rewind(VoidPtr node) {
  if (!node) return NO;
  reinterpret_cast<Ext2Node*>(node)->fCursor = 0;
  return YES;
}

VoidPtr Ext2FileSystemParser::Read(const Char* name, VoidPtr node, Int32 flags, SizeT size) {
  NE_UNUSED(name);
  return Read(node, flags, size);
}

Void Ext2FileSystemParser::Write(VoidPtr, VoidPtr, Int32, SizeT) {
  err_local_get() = kErrorUnavailable;
}

Void Ext2FileSystemParser::Write(const Char*, VoidPtr, VoidPtr, Int32, SizeT) {
  err_local_get() = kErrorUnavailable;
}

VoidPtr Ext2FileSystemParser::Create(const Char*) {
  err_local_get() = kErrorUnavailable;
  return nullptr;
}

VoidPtr Ext2FileSystemParser::CreateDirectory(const Char*) {
  err_local_get() = kErrorUnavailable;
  return nullptr;
}

Void Ext2FileSystemParser::Close(VoidPtr node) {
  if (node) mm_free_ptr(node);
}

BOOL Ext2FileSystemParser::GetInfo(VoidPtr node, FILEMGR_STAT* out) {
  if (!node || !out || !fCtx || !fCtx->fSuperblock) {
    err_local_get() = kErrorInvalidData;
    return NO;
  }

  auto n = reinterpret_cast<Ext2Node*>(node);
  const EXT2_INODE& ino = n->fInode;

  // widen to nanoseconds for VFS callers.
  const UInt64 kNs = 1000000000ULL;
  out->fCreationTime   = static_cast<UInt64>(ino.fCreateTime) * kNs;
  out->fLastAccessTime = static_cast<UInt64>(ino.fAccessTime) * kNs;
  out->fLastWriteTime  = static_cast<UInt64>(ino.fModifyTime) * kNs;
  out->fChangeTime     = out->fLastWriteTime;

  // fBlocks is in 512-byte sectors on ext2, not filesystem blocks.
  out->fAllocationSize = static_cast<UInt64>(ino.fBlocks) * 512ULL;
  out->fEndOfFile      = ext2_inode_size(ino, fCtx->fSuperblock->fFeatureROCompat);

  out->fNumberOfLinks  = ino.fLinksCount;
  out->fInodeNumber    = n->fInodeNumber;

  out->fMode           = ino.fMode & 0x0FFFu;
  out->fUID            = ino.fUID;
  out->fGID            = ino.fGID;

  out->fType           = ext2_inode_type(ino);
  out->fDirectory      = (out->fType == kExt2FileTypeDirectory) ? 1 : 0;
  out->fDeletePending  = 0;
  out->fReserved       = 0;

  return YES;
}

Int32 Ext2FileSystemParser::ReadLink(VoidPtr node, Char* buf, SizeT buf_size) {
  if (!node || !buf || buf_size == 0 || !fCtx || !fCtx->fSuperblock) {
    err_local_get() = kErrorInvalidData;
    return -1;
  }

  auto n = reinterpret_cast<Ext2Node*>(node);
  if (ext2_inode_type(n->fInode) != kExt2FileTypeSymbolicLink) {
    err_local_get() = kErrorInvalidData;
    return -1;
  }

  const UInt64 size = ext2_inode_size(n->fInode,
                                      fCtx->fSuperblock->fFeatureROCompat);
  if (size == 0) {
    err_local_get() = kErrorDiskIsCorrupted;
    return -1;
  }
  // Caller must reserve one byte for the terminator we write.
  if (size >= buf_size) {
    err_local_get() = kErrorInvalidData;
    return -1;
  }

  // Fast symlink: target lives in i_block[] when size < 60. Strictly less
  // than, not <=; e2fsck rejects exactly-60-byte inline symlinks.
  if (size < 60) {
    const Char* src = reinterpret_cast<const Char*>(&n->fInode.fBlock[0]);
    rt_copy_memory_safe(const_cast<Char*>(src), buf,
                        static_cast<SizeT>(size), buf_size);
    buf[size] = '\0';
    return static_cast<Int32>(size);
  }

  const SizeT bs = ext2_block_size(fCtx);
  // Symlinks longer than one FS block would need multi-block reads.
  // Not legal in any real ext2 volume (PATH_MAX << block size); refuse.
  if (size > bs) {
    err_local_get() = kErrorDiskIsCorrupted;
    return -1;
  }

  auto lk = ext2_lookup_block(fCtx, n, 0);
  if (lk.HasError()) {
    err_local_get() = lk.Error();
    return -1;
  }
  const block_lookup bl = lk.Leak().Leak();
  if (bl.phys == 0) {
    err_local_get() = kErrorDiskIsCorrupted;
    return -1;
  }

  UInt8* blk = ext2_cache_get(fCtx, bl.phys);
  if (!blk) {
    err_local_get() = kErrorDisk;
    return -1;
  }

  rt_copy_memory_safe(blk, buf, static_cast<SizeT>(size), buf_size);
  buf[size] = '\0';
  ext2_cache_put(fCtx, blk);
  return static_cast<Int32>(size);
}

BOOL Ext2FileSystemParser::ReadDir(VoidPtr node, UInt64 cookie, FILEMGR_DIRENT* out,
                                    UInt64* next_cookie) {
  if (!node || !out || !next_cookie || !fCtx || !fCtx->fSuperblock) {
    err_local_get() = kErrorInvalidData;
    return NO;
  }

  auto dir = reinterpret_cast<Ext2Node*>(node);
  if (ext2_inode_type(dir->fInode) != kExt2FileTypeDirectory) {
    err_local_get() = kErrorDirectoryNotFound;
    return NO;
  }

  const SizeT  bs    = ext2_block_size(fCtx);
  const UInt64 size  = ext2_inode_size(dir->fInode,
                                       fCtx->fSuperblock->fFeatureROCompat);
  const UInt64 nblk  = (size + bs - 1) / bs;
  const BOOL   hasFT = (fCtx->fSuperblock->fRevisionLevel >= kExt2FSRev1) &&
                       ((fCtx->fSuperblock->fFeatureIncompat &
                         kExt2FeatureIncompatFileType) != 0);

  UInt64 blkIdx = cookie >> 32;
  UInt32 ofs    = static_cast<UInt32>(cookie & 0xFFFFFFFFu);

  if (blkIdx >= nblk) {
    err_local_get() = kErrorSuccess;
    return NO;
  }

  while (blkIdx < nblk) {
    auto lk = ext2_lookup_block(fCtx, dir, blkIdx);
    if (lk.HasError()) {
      err_local_get() = lk.Error();
      return NO;
    }
    const block_lookup bl = lk.Leak().Leak();

    if (bl.phys == 0) {
      ++blkIdx;
      ofs = 0;
      continue;
    }

    UInt8* buf = ext2_cache_get(fCtx, bl.phys);
    if (!buf) {
      err_local_get() = kErrorDisk;
      return NO;
    }

    while (ofs < bs) {
      const UInt16 rec = ext2_dir_entry_ok(buf, ofs, static_cast<UInt32>(bs));
      if (rec == 0) break;

      auto h = reinterpret_cast<const EXT2_DIR_ENTRY_HEADER*>(buf + ofs);

      if (h->fInode != 0) {
        const UInt8 nameLen = h->fNameLength;
        const Char* name    = reinterpret_cast<const Char*>(
            buf + ofs + kExt2FSDirEntryHeaderLen);

        out->fInodeNumber = h->fInode;
        out->fType        = hasFT ? h->fFileType : static_cast<UInt8>(kExt2FileTypeUnknown);
        out->fNameLength  = nameLen;
        out->fReserved[0] = 0;
        out->fReserved[1] = 0;

        rt_copy_memory_safe(const_cast<Char*>(name), out->fName, nameLen,
                            sizeof(out->fName) - 1);
        out->fName[nameLen] = '\0';

        const UInt32 nextOfs = ofs + rec;
        if (nextOfs >= bs) {
          *next_cookie = (blkIdx + 1) << 32;
        } else {
          *next_cookie = (blkIdx << 32) | nextOfs;
        }

        ext2_cache_put(fCtx, buf);
        return YES;
      }

      ofs += rec;
    }

    ext2_cache_put(fCtx, buf);
    ++blkIdx;
    ofs = 0;
  }

  err_local_get() = kErrorSuccess;
  return NO;
}

}  // namespace Ne::Kernel

// The previous impl had (a) double cursor advance on Read,
// (b) triple-indirect writes that silently dropped data and double-freed,
// (c) no ordering between data / indirect / inode / bitmap / GDT writes.
// Restoring write support needs a block allocator, a journal replay path
// for HAS_JOURNAL (ext3) volumes, and an ordered commit protocol. Until
// then, every write path returns kErrorUnavailable it is what it is.

#endif
#endif
