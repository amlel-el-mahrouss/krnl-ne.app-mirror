// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef FSKIT_EXT2_IFS_H
#define FSKIT_EXT2_IFS_H

#include <FSKit/Ext2.h>
#include <KernelKit/DebugOutput.h>
#include <KernelKit/DriveMgr.h>
#include <KernelKit/HeapMgr.h>
#include <KernelKit/KPC.h>
#include <NeKit/ErrorOr.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/Utils.h>

namespace Kernel {

struct FILEMGR_STAT;
struct FILEMGR_DIRENT;

inline constexpr UInt32 kExt2CacheSlots = 64;

struct Ext2CacheSlot final {
  UInt32 fBlock;
  UInt32 fRefCount;
  UInt64 fAge;
  UInt8  fValid;
  UInt8  fReserved[3];
};

struct Ext2Context final {
  DriveTrait*       fDrive;
  EXT2_SUPER_BLOCK* fSuperblock;
  Ext2CacheSlot     fCacheSlots[kExt2CacheSlots];
  UInt8*            fCacheData;
  UInt64            fCacheAge;

  explicit Ext2Context(DriveTrait* d)
      : fDrive(d), fSuperblock(nullptr), fCacheData(nullptr), fCacheAge(0) {
    for (UInt32 i = 0; i < kExt2CacheSlots; ++i) {
      fCacheSlots[i].fBlock    = 0;
      fCacheSlots[i].fRefCount = 0;
      fCacheSlots[i].fAge      = 0;
      fCacheSlots[i].fValid    = 0;
      fCacheSlots[i].fReserved[0] = 0;
      fCacheSlots[i].fReserved[1] = 0;
      fCacheSlots[i].fReserved[2] = 0;
    }
  }

  ~Ext2Context() {
    if (fSuperblock) mm_free_ptr(fSuperblock);
    if (fCacheData)  mm_free_ptr(fCacheData);
  }

  NE_COPY_DELETE(Ext2Context)
};

inline SizeT ext2_block_size(const Ext2Context* c) {
  if (!c->fSuperblock) return kExt2FSBlockSizeBase;
  return static_cast<SizeT>(kExt2FSBlockSizeBase) << c->fSuperblock->fLogBlockSize;
}

inline UInt32 ext2_inode_record_size(const Ext2Context* c) {
  if (!c->fSuperblock || c->fSuperblock->fRevisionLevel < kExt2FSRev1 ||
      c->fSuperblock->fInodeSize == 0)
    return kExt2FSGoodOldInodeSize;
  return c->fSuperblock->fInodeSize;
}

// For 1 KiB block volumes the superblock sits in block 1 and the GDT
// follows in block 2. For larger block sizes both share block 0, and
// the GDT starts at block 1.
inline UInt32 ext2_gdt_first_block(const Ext2Context* c) {
  return (ext2_block_size(c) == kExt2FSBlockSizeBase) ? 2u : 1u;
}

inline UInt32 ext2_group_count(const Ext2Context* c) {
  if (!c->fSuperblock || c->fSuperblock->fBlocksPerGroup == 0) return 0;
  const UInt64 bc  = c->fSuperblock->fBlockCount;
  const UInt64 bpg = c->fSuperblock->fBlocksPerGroup;
  return static_cast<UInt32>((bc + bpg - 1) / bpg);
}

inline BOOL ext2_raw_read(DriveTrait* d, Lba lba, VoidPtr dst, SizeT bytes) {
  if (!d || !dst || bytes == 0) return NO;
  d->fPacket.fPacketContent  = dst;
  d->fPacket.fPacketSize     = bytes;
  d->fPacket.fPacketLba      = lba;
  d->fPacket.fPacketReadOnly = NO;
  d->fPacket.fPacketGood     = YES;
  d->fInput(d->fPacket);
  return d->fPacket.fPacketGood;
}

inline BOOL ext2_raw_write(DriveTrait* d, Lba lba, const VoidPtr src, SizeT bytes) {
  if (!d || !src || bytes == 0) return NO;
  d->fPacket.fPacketContent = const_cast<VoidPtr>(src);
  d->fPacket.fPacketSize    = bytes;
  d->fPacket.fPacketLba     = lba;
  d->fPacket.fPacketGood    = YES;
  d->fOutput(d->fPacket);
  return d->fPacket.fPacketGood;
}

// 64-bit LBA math: UInt32 * UInt32 overflows on >2 TiB volumes.
inline Lba ext2_block_to_lba(const Ext2Context* c, UInt32 blockNumber) {
  if (!c->fDrive || !c->fSuperblock) return 0;
  const SizeT  bs  = ext2_block_size(c);
  const SizeT  ss  = c->fDrive->fSectorSz ? c->fDrive->fSectorSz : kDriveSectorSz;
  const UInt64 spb = (bs + ss - 1) / ss;
  return static_cast<Lba>(blockNumber) * static_cast<Lba>(spb) + c->fDrive->fLbaStart;
}

// Block cache: init after superblock is loaded.
inline BOOL ext2_cache_init(Ext2Context* c) {
  const SizeT bs    = ext2_block_size(c);
  const SizeT total = bs * kExt2CacheSlots;
  c->fCacheData = static_cast<UInt8*>(mm_alloc_ptr(total, Yes, No));
  if (!c->fCacheData) return NO;
  rt_zero_memory(c->fCacheData, total);
  return YES;
}

// Returns pinned pointer. Caller MUST call ext2_cache_put when done.
inline UInt8* ext2_cache_get(Ext2Context* c, UInt32 blockNumber) {
  if (!c->fCacheData) return nullptr;
  const SizeT bs = ext2_block_size(c);

  for (UInt32 i = 0; i < kExt2CacheSlots; ++i) {
    if (c->fCacheSlots[i].fValid && c->fCacheSlots[i].fBlock == blockNumber) {
      ++c->fCacheSlots[i].fRefCount;
      c->fCacheSlots[i].fAge = ++c->fCacheAge;
      return c->fCacheData + (static_cast<SizeT>(i) * bs);
    }
  }

  UInt32 victim = 0xFFFFFFFFu;
  UInt64 oldest = ~0ULL;
  for (UInt32 i = 0; i < kExt2CacheSlots; ++i) {
    if (c->fCacheSlots[i].fRefCount == 0) {
      if (!c->fCacheSlots[i].fValid) {
        victim = i;
        break;
      }
      if (c->fCacheSlots[i].fAge < oldest) {
        oldest = c->fCacheSlots[i].fAge;
        victim = i;
      }
    }
  }

  if (victim == 0xFFFFFFFFu) return nullptr;

  UInt8* data = c->fCacheData + (static_cast<SizeT>(victim) * bs);

  if (!ext2_raw_read(c->fDrive, ext2_block_to_lba(c, blockNumber), data, bs))
    return nullptr;

  c->fCacheSlots[victim].fBlock    = blockNumber;
  c->fCacheSlots[victim].fRefCount = 1;
  c->fCacheSlots[victim].fAge      = ++c->fCacheAge;
  c->fCacheSlots[victim].fValid    = 1;
  return data;
}

inline void ext2_cache_put(Ext2Context* c, UInt8* ptr) {
  if (!c->fCacheData || !ptr) return;
  const SizeT bs     = ext2_block_size(c);
  const SizeT offset = static_cast<SizeT>(ptr - c->fCacheData);
  const UInt32 idx   = static_cast<UInt32>(offset / bs);
  if (idx < kExt2CacheSlots && c->fCacheSlots[idx].fRefCount > 0)
    --c->fCacheSlots[idx].fRefCount;
}

inline ErrorOr<EXT2_SUPER_BLOCK*> ext2_mount(Ext2Context* c) {
  if (!c->fDrive) return ErrorOr<EXT2_SUPER_BLOCK*>(kErrorInvalidData);

  auto sb = static_cast<EXT2_SUPER_BLOCK*>(
      mm_alloc_ptr(kExt2FSSuperblockSize, Yes, No));
  if (!sb) return ErrorOr<EXT2_SUPER_BLOCK*>(kErrorHeapOutOfMemory);

  const SizeT ss = c->fDrive->fSectorSz ? c->fDrive->fSectorSz : kDriveSectorSz;
  if (kExt2FSSuperblockOffset % ss != 0) {
    mm_free_ptr(sb);
    kout << "ext2: sector size does not align to superblock offset\r";
    return ErrorOr<EXT2_SUPER_BLOCK*>(kErrorInvalidData);
  }

  const Lba sbLba = c->fDrive->fLbaStart + (kExt2FSSuperblockOffset / ss);
  if (!ext2_raw_read(c->fDrive, sbLba, sb, kExt2FSSuperblockSize)) {
    mm_free_ptr(sb);
    return ErrorOr<EXT2_SUPER_BLOCK*>(kErrorDisk);
  }

  if (sb->fMagic != kExt2FSMagic) {
    mm_free_ptr(sb);
    return ErrorOr<EXT2_SUPER_BLOCK*>(kErrorInvalidData);
  }

  if (sb->fInodesPerGroup == 0 || sb->fBlocksPerGroup == 0 ||
      sb->fInodeCount == 0 || sb->fBlockCount == 0) {
    mm_free_ptr(sb);
    kout << "ext2: degenerate superblock\r";
    return ErrorOr<EXT2_SUPER_BLOCK*>(kErrorDiskIsCorrupted);
  }

  if (sb->fLogBlockSize > 6) {
    mm_free_ptr(sb);
    kout << "ext2: log block size out of range\r";
    return ErrorOr<EXT2_SUPER_BLOCK*>(kErrorDiskIsCorrupted);
  }

  if (sb->fRevisionLevel >= kExt2FSRev1) {
    if ((sb->fFeatureIncompat & ~kExt2FSIncompatSupported) != 0) {
      mm_free_ptr(sb);
      kout << "ext2: unsupported INCOMPAT bits\r";
      return ErrorOr<EXT2_SUPER_BLOCK*>(kErrorInvalidData);
    }
  }

  c->fSuperblock = sb;
  if (!ext2_cache_init(c)) {
    c->fSuperblock = nullptr;
    mm_free_ptr(sb);
    return ErrorOr<EXT2_SUPER_BLOCK*>(kErrorHeapOutOfMemory);
  }
  return ErrorOr<EXT2_SUPER_BLOCK*>(sb);
}

inline ErrorOr<UInt32> ext2_read_group_desc(Ext2Context* c, UInt32 inodeNumber,
                                             EXT2_GROUP_DESCRIPTOR* out) {
  if (!c->fSuperblock || !out || inodeNumber == 0)
    return ErrorOr<UInt32>(kErrorInvalidData);

  const UInt32 group = (inodeNumber - 1) / c->fSuperblock->fInodesPerGroup;
  if (group >= ext2_group_count(c)) return ErrorOr<UInt32>(kErrorInvalidData);

  const SizeT  bs     = ext2_block_size(c);
  const UInt64 byte   = static_cast<UInt64>(group) * sizeof(EXT2_GROUP_DESCRIPTOR);
  const UInt32 blkIdx = static_cast<UInt32>(byte / bs);
  const UInt32 ofs    = static_cast<UInt32>(byte % bs);
  const UInt32 target = ext2_gdt_first_block(c) + blkIdx;

  UInt8* blk = ext2_cache_get(c, target);
  if (!blk) return ErrorOr<UInt32>(kErrorDisk);

  rt_copy_memory_safe(blk + ofs, out, sizeof(*out), sizeof(*out));
  ext2_cache_put(c, blk);
  return ErrorOr<UInt32>(group);
}

inline ErrorOr<Ext2Node*> ext2_load_inode(Ext2Context* c, UInt32 inodeNumber) {
  if (!c->fSuperblock || inodeNumber == 0 ||
      inodeNumber > c->fSuperblock->fInodeCount)
    return ErrorOr<Ext2Node*>(kErrorInvalidData);

  EXT2_GROUP_DESCRIPTOR desc{};
  auto gr = ext2_read_group_desc(c, inodeNumber, &desc);
  if (gr.HasError()) return ErrorOr<Ext2Node*>(gr.Error());

  const SizeT  bs  = ext2_block_size(c);
  const UInt32 isz = ext2_inode_record_size(c);
  const UInt32 ipg = c->fSuperblock->fInodesPerGroup;
  const UInt32 idx = (inodeNumber - 1) % ipg;
  const UInt32 ipb = bs / isz;
  if (ipb == 0) return ErrorOr<Ext2Node*>(kErrorDiskIsCorrupted);

  const UInt32 blkInTable = idx / ipb;
  const UInt32 ofs        = (idx % ipb) * isz;
  const UInt32 target     = desc.fInodeTable + blkInTable;

  UInt8* blk = ext2_cache_get(c, target);
  if (!blk) return ErrorOr<Ext2Node*>(kErrorDisk);

  auto n = static_cast<Ext2Node*>(mm_alloc_ptr(sizeof(Ext2Node), Yes, No));
  if (!n) {
    ext2_cache_put(c, blk);
    return ErrorOr<Ext2Node*>(kErrorHeapOutOfMemory);
  }

  n->fInodeNumber = inodeNumber;
  n->fCursor      = 0;
  rt_copy_memory_safe(blk + ofs, &n->fInode, sizeof(EXT2_INODE), sizeof(EXT2_INODE));
  ext2_cache_put(c, blk);
  return ErrorOr<Ext2Node*>(n);
}

class Ext2FileSystemParser final {
 public:
  explicit Ext2FileSystemParser(DriveTrait* drive);
  ~Ext2FileSystemParser();

  NE_COPY_DELETE(Ext2FileSystemParser)

  VoidPtr Open(const Char* path, const Char* restrict_type);
  VoidPtr Read(VoidPtr node, Int32 flags, SizeT size);
  VoidPtr Read(const Char* name, VoidPtr node, Int32 flags, SizeT size);

  Void    Write(VoidPtr node, VoidPtr data, Int32 flags, SizeT size);
  Void    Write(const Char* name, VoidPtr node, VoidPtr data, Int32 flags, SizeT size);
  VoidPtr Create(const Char* path);
  VoidPtr CreateDirectory(const Char* path);

  BOOL  Seek(VoidPtr node, SizeT offset);
  SizeT Tell(VoidPtr node);
  BOOL  Rewind(VoidPtr node);

  Void  Close(VoidPtr node);
  BOOL  GetInfo(VoidPtr node, FILEMGR_STAT* out);
  BOOL  ReadDir(VoidPtr node, UInt64 cookie, FILEMGR_DIRENT* out, UInt64* next_cookie);
  Int32 ReadLink(VoidPtr node, Char* buf, SizeT buf_size);

  Ext2Context* Context() { return fCtx; }

 private:
  Ext2Context* fCtx;
};

}  // namespace Kernel

#endif
