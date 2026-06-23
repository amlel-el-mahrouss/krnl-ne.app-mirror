// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef __NE_MINIMAL_OS__
#ifdef __FSKIT_INCLUDES_OPENHEFS__

#include <KernelKit/FileMgr.h>
#include <KernelKit/HeapMgr.h>

/// @brief OpenHeFS File System Manager.
/// BUGS: 0

namespace Ne::Kernel {

/// @brief C++ constructor
HeFileSystemMgr::HeFileSystemMgr() {
  mParser = new HeFileSystemParser();
  MUST_PASS(mParser);

  kout << "OpenHeFS: Allocated HeFileSystemParser...\r";
}

HeFileSystemMgr::~HeFileSystemMgr() {
  if (mParser) {
    kout << "OpenHeFS: Destroying HeFileSystemParser...\r";

    delete mParser;
    mParser = nullptr;
  }
}

/// @brief Removes a node from the filesystem.
/// @param path The filename
/// @return If it was deleted or not.
bool HeFileSystemMgr::Remove(_Input const Char* path) {
  if (path == nullptr || *path == 0) {
    kout << "OpenHeFS: Remove called with null or empty path\r";
    return NO;
  }

  auto len = oe_string_len<Char>(path);

  if (len == 0) return NO;

  Utf8Char* out = static_cast<Utf8Char*>(RTL_ALLOCA(sizeof(Utf8Char) * len));

  err_local_get() = kErrorSuccess;

  bool ret = mParser->DeleteINode(&mDriveTrait, 0, nullptr, out, 0);
  return ret;
}

/// @brief Creates a node with the specified.
/// @param path The filename path.
/// @return The Node pointer.
NodePtr HeFileSystemMgr::Create(_Input const Char* path) {
  if (!path || *path == 0) {
    kout << "OpenHeFS: Create called with null or empty path\r";
    return nullptr;
  }

  // TODO: It needs its own helper!
  SizeT len = oe_string_len<Char>(path);

  if (len == 0) return nullptr;

  Utf8Char* out = static_cast<Utf8Char*>(RTL_ALLOCA(sizeof(Utf8Char) * len));

  for (SizeT indx = 0UL; indx < len; ++indx) {
    out[indx] = path[indx];
  }

  err_local_get() = kErrorSuccess;

  if (auto node = mParser->CreateINode(&mDriveTrait, 0, nullptr, out, 0); node) return nullptr;

  kout << "OpenHeFS: ERROR: Check KPC.\r";

  err_local_get() = kErrorDiskIsFull;

  return nullptr;
}

/// @brief Creates a node which is a directory.
/// @param path The filename path.
/// @return The Node pointer.
NodePtr HeFileSystemMgr::CreateDirectory(const Char* path) {
  if (!path || *path == 0) {
    kout << "OpenHeFS: CreateDirectory called with null or empty path\r";
    return nullptr;
  }

  // TODO: It needs its own helper!
  SizeT len = oe_string_len<Char>(path);

  if (len == 0) return nullptr;

  Utf8Char* out = static_cast<Utf8Char*>(RTL_ALLOCA(sizeof(Utf8Char) * len));

  for (SizeT indx = 0UL; indx < len; ++indx) {
    out[indx] = path[indx];
  }

  err_local_get() = kErrorSuccess;

  if (auto node = mParser->CreateINodeDirectory(&mDriveTrait, 0, out); node) return nullptr;

  kout << "OpenHeFS: ERROR: Check KPC.\r";

  err_local_get() = kErrorDiskIsFull;

  return nullptr;
}

/// @brief Creates a node which is an alias.
/// @param path The filename path.
/// @return The Node pointer.
NodePtr HeFileSystemMgr::CreateAlias(const Char* path) {
  if (!path || *path == 0) {
    kout << "OpenHeFS: CreateAlias called with null or empty path\r";
    return nullptr;
  }

  // TODO: It needs its own helper!
  SizeT len = oe_string_len<Char>(path);

  if (len == 0) return nullptr;

  Utf8Char* out = static_cast<Utf8Char*>(RTL_ALLOCA(sizeof(Utf8Char) * len));

  for (SizeT indx = 0UL; indx < len; ++indx) {
    out[indx] = path[indx];
  }

  err_local_get() = kErrorSuccess;

  if (auto node =
          mParser->CreateINode(&mDriveTrait, kOpenHeFSFileKindSymbolicLink, nullptr, out, 0);
      node)
    return nullptr;

  kout << "OpenHeFS: ERROR: Check KPC.\r";

  err_local_get() = kErrorDiskIsFull;

  return nullptr;
}

NodePtr HeFileSystemMgr::CreateSwapFile(const Char* path) {
  if (!path || *path == 0) {
    kout << "OpenHeFS: CreateSwapFile called with null or empty path\r";
    return nullptr;
  }

  kout << "OpenHEFS: ERROR: Swap Files are not supported natively by OpenHeFS.\r";
  err_local_get() = kErrorInvalidData;

  return nullptr;
}

/// @brief Gets the root directory.
/// @return
const Char* NeFileSystemHelper::Root() {
  return kOpenHeFSRootDirectory;
}

/// @brief Gets the up-dir directory.
/// @return
const Char* NeFileSystemHelper::UpDir() {
  return kOpenHeFSUpDir;
}

/// @brief Gets the separator character.
/// @return
Char NeFileSystemHelper::Separator() {
  return kOpenHeFSSeparator;
}

/// @brief Gets the metafile character.
/// @return
Char NeFileSystemHelper::MetaFile() {
  return '\0';
}

/// @brief Opens a new file.
/// @param path
/// @param r
/// @return
_Output NodePtr HeFileSystemMgr::Open(_Input const Char* path, _Input const Char* r) {
  if (!path || *path == 0) {
    kout << "OpenHeFS: Open called with null or empty path\r";
    return nullptr;
  }

  if (!r || *r == 0) {
    kout << "OpenHeFS: Open called with null or empty mode string\r";
    return nullptr;
  }

  return nullptr;
}

Void HeFileSystemMgr::Write(_Input NodePtr node, _Input VoidPtr data, _Input Int32 flags,
                            _Input SizeT size) {
  NE_UNUSED(node);
  NE_UNUSED(flags);
  NE_UNUSED(size);
  NE_UNUSED(data);
}

_Output VoidPtr HeFileSystemMgr::Read(_Input NodePtr node, _Input Int32 flags, _Input SizeT size) {
  NE_UNUSED(flags);

  if (!node || !size) return nullptr;

  return nullptr;
}

/// @note name is not used in OpenHeFS to mark data offsets. That's an NeFS-ism.
Void HeFileSystemMgr::Write(_Input const Char* name, _Input NodePtr node, _Input VoidPtr data,
                            _Input Int32 flags, _Input SizeT size) {
  NE_UNUSED(node);

  if (!flags) return;
  if (!size) return;
  if (!data) return;
  if (!name) return;

  STATIC IMountpoint mnt;
  io_construct_main_drive(mnt.A());

  mParser->INodeManip(&mnt.A(), (VoidPtr) data, size, u8"/", (Char8*) name, 0, NO);
}

_Output VoidPtr HeFileSystemMgr::Read(_Input const Char* name, _Input NodePtr node,
                                      _Input Int32 flags, _Input SizeT sz) {
  NE_UNUSED(node);
  NE_UNUSED(flags);
  NE_UNUSED(sz);
  NE_UNUSED(name);

  UInt8* retBlob = new UInt8[sz];

  if (!retBlob) return nullptr;

  rt_set_memory(retBlob, 0, sz);

  STATIC IMountpoint mnt;
  io_construct_main_drive(mnt.A());

  mParser->INodeManip(&mnt.A(), (VoidPtr) retBlob, sz, u8"/", (Char8*) name, 0, YES);

  return retBlob;
}

_Output Bool HeFileSystemMgr::Seek(NodePtr node, SizeT off) {
  if (this->Tell(node) == kFileMgrNPos) return false;
  kout << "The Method is not implemented in the Hybrid kernel.\r";
  return off > 0;
}

/// @brief Tell current offset within catalog.
/// @param node The HeFS node we need.
/// @return kFileMgrNPos if invalid, else current offset.
_Output SizeT HeFileSystemMgr::Tell(NodePtr node) {
  if (!node) return kFileMgrNPos;
  SizeT pos = 0ULL;
  kout << "The Method is not implemented in the Hybrid kernel.\r";
  return pos;
}

/// @brief Rewinds the catalog
/// @param node The needed HeFS node.
/// @return False if invalid, nah? calls Seek(node, 0).
_Output Bool HeFileSystemMgr::Rewind(NodePtr node) {
  if (!node) return false;
  return true;
}

/// @brief Returns the parser of OpenHeFS.
_Output HeFileSystemParser* HeFileSystemMgr::GetParser() {
  return mParser;
}

}  // namespace Ne::Kernel

#endif  // ifdef __FSKIT_INCLUDES_OPENHEFS__
#endif  // ifndef __NE_MINIMAL_OS__
