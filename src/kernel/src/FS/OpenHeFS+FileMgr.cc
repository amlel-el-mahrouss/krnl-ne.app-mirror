// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef __NE_MINIMAL_OS__
#ifdef __FSKIT_INCLUDES_OPENHEFS__

#include <KernelKit/FileMgr.h>
#include <KernelKit/HeapMgr.h>

/// @brief OpenHeFS File System Manager.
/// BUGS: 0

namespace Kernel {
/// @brief C++ constructor
HeFileSystemMgr::HeFileSystemMgr() {
  mParser = new HeFileSystemParser();
  MUST_PASS(mParser);

  kout << "We are done allocating HeFileSystemParser...\n";
}

HeFileSystemMgr::~HeFileSystemMgr() {
  if (mParser) {
    kout << "Destroying HeFileSystemParser...\n";
    delete mParser;
    mParser = nullptr;
  }
}

/// @brief Removes a node from the filesystem.
/// @param path The filename
/// @return If it was deleted or not.
bool HeFileSystemMgr::Remove(_Input const Char* path) {
  if (path == nullptr || *path == 0) {
    kout << "OpenHeFS: Remove called with null or empty path\n";
    return false;
  }

  return NO;
}

/// @brief Creates a node with the specified.
/// @param path The filename path.
/// @return The Node pointer.
NodePtr HeFileSystemMgr::Create(_Input const Char* path) {
  if (!path || *path == 0) {
    kout << "OpenHeFS: Create called with null or empty path\n";
    return nullptr;
  }

  // AMLALE: TODO, its own helper!
  SizeT len = oe_string_len<Char>(path);

#if defined(__clang__)
  Utf8Char out[len];
  rt_set_memory(out, 0, len);
#else
  Utf8Char* out = static_cast<Utf8Char*>(RTL_ALLOCA(sizeof(Utf8Char) * len));
#endif

  for (SizeT indx = 0UL; indx < len; ++indx) {
    out[indx] = path[indx];
  }

  if (mParser->CreateINode(&mDriveTrait, 0, nullptr, out, 0))
    return nullptr;  // AMLALE TODO: FetchINode method!

  return nullptr;
}

/// @brief Creates a node which is a directory.
/// @param path The filename path.
/// @return The Node pointer.
NodePtr HeFileSystemMgr::CreateDirectory(const Char* path) {
  if (!path || *path == 0) {
    kout << "OpenHeFS: CreateDirectory called with null or empty path\n";
    return nullptr;
  }
  return nullptr;
}

/// @brief Creates a node which is an alias.
/// @param path The filename path.
/// @return The Node pointer.
NodePtr HeFileSystemMgr::CreateAlias(const Char* path) {
  if (!path || *path == 0) {
    kout << "OpenHeFS: CreateAlias called with null or empty path\n";
    return nullptr;
  }
  return nullptr;
}

NodePtr HeFileSystemMgr::CreateSwapFile(const Char* path) {
  if (!path || *path == 0) {
    kout << "OpenHeFS: CreateSwapFile called with null or empty path\n";
    return nullptr;
  }
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
    kout << "OpenHeFS: Open called with null or empty path\n";
    return nullptr;
  }
  if (!r || *r == 0) {
    kout << "OpenHeFS: Open called with null or empty mode string\n";
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
  NE_UNUSED(node);
  NE_UNUSED(flags);
  NE_UNUSED(size);

  return nullptr;
}

Void HeFileSystemMgr::Write(_Input const Char* name, _Input NodePtr node, _Input VoidPtr data,
                            _Input Int32 flags, _Input SizeT size) {
  NE_UNUSED(node);
  NE_UNUSED(flags);
  NE_UNUSED(size);
  NE_UNUSED(name);
  NE_UNUSED(data);
}

_Output VoidPtr HeFileSystemMgr::Read(_Input const Char* name, _Input NodePtr node,
                                      _Input Int32 flags, _Input SizeT sz) {
  NE_UNUSED(node);
  NE_UNUSED(flags);
  NE_UNUSED(sz);
  NE_UNUSED(name);

  return nullptr;
}

_Output Bool HeFileSystemMgr::Seek(NodePtr node, SizeT off) {
  NE_UNUSED(node);
  NE_UNUSED(off);

  return false;
}

/// @brief Tell current offset within catalog.
/// @param node
/// @return kFileMgrNPos if invalid, else current offset.
_Output SizeT HeFileSystemMgr::Tell(NodePtr node) {
  NE_UNUSED(node);
  return kFileMgrNPos;
}

/// @brief Rewinds the catalog
/// @param node
/// @return False if invalid, nah? calls Seek(node, 0).
_Output Bool HeFileSystemMgr::Rewind(NodePtr node) {
  NE_UNUSED(node);
  return kFileMgrNPos;
}

/// @brief Returns the parser of OpenHeFS.
_Output HeFileSystemParser* HeFileSystemMgr::GetParser() {
  return mParser;
}
}  // namespace Kernel

#endif  // ifdef __FSKIT_INCLUDES_OPENHEFS__
#endif  // ifndef __NE_MINIMAL_OS__
