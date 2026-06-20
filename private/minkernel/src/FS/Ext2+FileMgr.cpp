// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef __NE_MINIMAL_OS__
#ifdef __FSKIT_INCLUDES_EXT2__

#include <FSKit/Ext2+IFS.h>
#include <FSKit/Ext2.h>
#include <KernelKit/DebugOutput.h>
#include <KernelKit/DriveMgr.h>
#include <KernelKit/FileMgr.h>
#include <KernelKit/HeapMgr.h>

namespace Kernel {

Ext2FileSystemMgr::Ext2FileSystemMgr() {
  io_construct_main_drive(mDriveTrait);
  mParser = new Ext2FileSystemParser(&mDriveTrait);
  MUST_PASS(mParser);
}

Ext2FileSystemMgr::~Ext2FileSystemMgr() {
  if (mParser) {
    delete mParser;
    mParser = nullptr;
  }
}

NodePtr Ext2FileSystemMgr::Create(const Char*) {
  err_local_get() = kErrorUnavailable;
  return nullptr;
}

NodePtr Ext2FileSystemMgr::CreateAlias(const Char*) {
  err_local_get() = kErrorUnavailable;
  return nullptr;
}

NodePtr Ext2FileSystemMgr::CreateDirectory(const Char*) {
  err_local_get() = kErrorUnavailable;
  return nullptr;
}

NodePtr Ext2FileSystemMgr::CreateSwapFile(const Char*) {
  err_local_get() = kErrorUnavailable;
  return nullptr;
}

bool Ext2FileSystemMgr::Remove(_Input const Char*) {
  err_local_get() = kErrorUnavailable;
  return false;
}

NodePtr Ext2FileSystemMgr::Open(_Input const Char* path, _Input const Char* r) {
  if (!mParser || !path || !*path || !r || !*r) {
    err_local_get() = kErrorInvalidData;
    return nullptr;
  }
  return mParser->Open(path, r);
}

Void Ext2FileSystemMgr::Write(_Input NodePtr, _Input VoidPtr, _Input Int32, _Input SizeT) {
  err_local_get() = kErrorUnavailable;
}

VoidPtr Ext2FileSystemMgr::Read(_Input NodePtr node, _Input Int32 flags, _Input SizeT sz) {
  if (!mParser) return nullptr;
  return mParser->Read(node, flags, sz);
}

bool Ext2FileSystemMgr::Seek(_Input NodePtr node, _Input SizeT off) {
  if (!mParser) return false;
  return mParser->Seek(node, off);
}

SizeT Ext2FileSystemMgr::Tell(_Input NodePtr node) {
  if (!mParser) return kFileMgrNPos;
  return mParser->Tell(node);
}

bool Ext2FileSystemMgr::Rewind(_Input NodePtr node) {
  if (!mParser) return false;
  return mParser->Rewind(node);
}

Void Ext2FileSystemMgr::Write(_Input const Char*, _Input NodePtr, _Input VoidPtr,
                              _Input Int32, _Input SizeT) {
  err_local_get() = kErrorUnavailable;
}

_Output VoidPtr Ext2FileSystemMgr::Read(_Input const Char* name, _Input NodePtr node,
                                        _Input Int32 flags, _Input SizeT sz) {
  if (!mParser) return nullptr;
  return mParser->Read(name, node, flags, sz);
}

BOOL Ext2FileSystemMgr::GetInfo(_Input NodePtr node, _Output FILEMGR_STAT* out) {
  if (!mParser) {
    err_local_get() = kErrorUnavailable;
    return NO;
  }
  return mParser->GetInfo(node, out);
}

BOOL Ext2FileSystemMgr::ReadDir(_Input NodePtr node, _Input UInt64 cookie,
                                 _Output FILEMGR_DIRENT* out,
                                 _Output UInt64* next_cookie) {
  if (!mParser) {
    err_local_get() = kErrorUnavailable;
    return NO;
  }
  return mParser->ReadDir(node, cookie, out, next_cookie);
}

Int32 Ext2FileSystemMgr::ReadLink(_Input NodePtr node, _Output Char* buf,
                                   _Input SizeT buf_size) {
  if (!mParser) {
    err_local_get() = kErrorUnavailable;
    return -1;
  }
  return mParser->ReadLink(node, buf, buf_size);
}

Ext2FileSystemParser* Ext2FileSystemMgr::GetParser() { return mParser; }

}  // namespace Kernel

#endif
#endif
