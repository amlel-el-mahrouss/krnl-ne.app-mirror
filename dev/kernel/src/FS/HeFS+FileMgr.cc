/* -------------------------------------------

  Copyright (C) 2024-2025, Amlal El Mahrouss, all rights reserved.

------------------------------------------- */

#ifndef __NE_MINIMAL_OS__
#ifdef __FSKIT_INCLUDES_HEFS__

#include <KernelKit/FileMgr.h>
#include <KernelKit/HeapMgr.h>

/// @brief HeFS File System Manager.
/// BUGS: 0

namespace Kernel {
/// @brief C++ constructor
HeFileSystemMgr::HeFileSystemMgr() {
  mParser = new HeFileSystemParser();
  MUST_PASS(mParser);

  kout << "We are done allocating NeFileSystemParser...\n";
}

HeFileSystemMgr::~HeFileSystemMgr() {
  if (mParser) {
    kout << "Destroying NeFileSystemParser...\n";
    delete mParser;
    mParser = nullptr;
  }
}

/// @brief Removes a node from the filesystem.
/// @param path The filename
/// @return If it was deleted or not.
bool HeFileSystemMgr::Remove(_Input const Char* path) {
  if (path == nullptr || *path == 0) {
    kout << "HeFS: Remove called with null or empty path\n";
    return false;
  }

  return NO;
}

/// @brief Creates a node with the specified.
/// @param path The filename path.
/// @return The Node pointer.
NodePtr HeFileSystemMgr::Create(_Input const Char* path) {
  if (!path || *path == 0) {
    kout << "HeFS: Create called with null or empty path\n";
    return nullptr;
  }
  return nullptr;
}

/// @brief Creates a node which is a directory.
/// @param path The filename path.
/// @return The Node pointer.
NodePtr HeFileSystemMgr::CreateDirectory(const Char* path) {
  if (!path || *path == 0) {
    kout << "HeFS: CreateDirectory called with null or empty path\n";
    return nullptr;
  }
  return nullptr;
}

/// @brief Creates a node which is an alias.
/// @param path The filename path.
/// @return The Node pointer.
NodePtr HeFileSystemMgr::CreateAlias(const Char* path) {
  if (!path || *path == 0) {
    kout << "HeFS: CreateAlias called with null or empty path\n";
    return nullptr;
  }
  return nullptr;
}

NodePtr HeFileSystemMgr::CreateSwapFile(const Char* path) {
  if (!path || *path == 0) {
    kout << "HeFS: CreateSwapFile called with null or empty path\n";
    return nullptr;
  }
  return nullptr;
}

/// @brief Gets the root directory.
/// @return
const Char* NeFileSystemHelper::Root() {
  return kHeFSRootDirectory;
}

/// @brief Gets the up-dir directory.
/// @return
const Char* NeFileSystemHelper::UpDir() {
  return kHeFSUpDir;
}

/// @brief Gets the separator character.
/// @return
Char NeFileSystemHelper::Separator() {
  return kHeFSSeparator;
}

/// @brief Gets the metafile character.
/// @return
Char NeFileSystemHelper::MetaFile() {
  return 0;
}

/// @brief Opens a new file.
/// @param path
/// @param r
/// @return
_Output NodePtr HeFileSystemMgr::Open(_Input const Char* path, _Input const Char* r) {
  if (!path || *path == 0) {
    kout << "HeFS: Open called with null or empty path\n";
    return nullptr;
  }
  if (!r || *r == 0) {
    kout << "HeFS: Open called with null or empty mode string\n";
    return nullptr;
  }
  return nullptr;
}

Void HeFileSystemMgr::Write(_Input NodePtr node, _Input VoidPtr data, _Input Int32 flags,
                            _Input SizeT size) {}

_Output VoidPtr HeFileSystemMgr::Read(_Input NodePtr node, _Input Int32 flags, _Input SizeT size) {
  return nullptr;
}

Void HeFileSystemMgr::Write(_Input const Char* name, _Input NodePtr node, _Input VoidPtr data,
                            _Input Int32 flags, _Input SizeT size) {}

_Output VoidPtr HeFileSystemMgr::Read(_Input const Char* name, _Input NodePtr node,
                                      _Input Int32 flags, _Input SizeT sz) {
  return nullptr;
}

_Output Bool HeFileSystemMgr::Seek(NodePtr node, SizeT off) {
  return false;
}

/// @brief Tell current offset within catalog.
/// @param node
/// @return kFileMgrNPos if invalid, else current offset.
_Output SizeT HeFileSystemMgr::Tell(NodePtr node) {
  return kFileMgrNPos;
}

/// @brief Rewinds the catalog
/// @param node
/// @return False if invalid, nah? calls Seek(node, 0).
_Output Bool HeFileSystemMgr::Rewind(NodePtr node) {
  return kFileMgrNPos;
}

/// @brief Returns the parser of HeFS.
_Output HeFileSystemParser* HeFileSystemMgr::GetParser() noexcept {
  return mParser;
}
}  // namespace Kernel

#endif  // ifdef __FSKIT_INCLUDES_HEFS__
#endif  // ifndef __NE_MINIMAL_OS__
