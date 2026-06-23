// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef KERNELKIT_CODEMGR_PEF_H
#define KERNELKIT_CODEMGR_PEF_H

/// @file PEFCodeMgr.h
/// @brief PEF Code Manager header file.
/// @author Amlal El Mahrouss (amlal@nekernel.org)

#include <KernelKit/FileMgr.h>
#include <KernelKit/PEF.h>
#include <NeKit/ErrorOr.h>
#include <NeKit/KString.h>

#ifndef KERNELKIT_USERPROCESSSCHEDULER_H
#include <KernelKit/ProcessScheduler.h>
#endif

#define kPefApplicationMime "application/vnd-nekernel-executable"

namespace Ne::Kernel {

///
/// \name PEFLoader
/// \brief PEF loader class.
///
class PEFLoader NE_EXEC_LOADER {
 private:
  explicit PEFLoader() = delete;

 public:
  explicit PEFLoader(const VoidPtr blob);
  explicit PEFLoader(const Char* path);
  ~PEFLoader() override;

 public:
  NE_COPY_DEFAULT(PEFLoader)

 public:
  const Char* Path() override;
  const Char* AsString() override;
  const Char* MIME() override;

 public:
  ErrorOr<VoidPtr> FindStart() override;
  ErrorOr<VoidPtr> FindSymbol(const Char* name, Int32 kind) override;
  ErrorOr<VoidPtr> GetBlob() override;

 public:
  bool IsLoaded();

 private:
#ifdef __FSKIT_INCLUDES_NEFS__
  OwnPtr<FileStream<Char, NeFileSystemMgr>> fFile;
#elif defined(__FSKIT_INCLUDES_OPENHEFS__)
  OwnPtr<FileStream<Char, HeFileSystemMgr>> fFile;
#else
  OwnPtr<FileStream<Char>> fFile;
#endif  // __FSKIT_INCLUDES_NEFS__

  Ref<KString> fPath;
  ErrorOrAny   fCachedBlob;
  BOOL         fFatBinary{};
  BOOL         fBad{};
};

ProcessID rtl_create_user_process(PEFLoader& exec, const UserProcess::ExecutableKind& procKind);

}  // namespace Ne::Kernel

#endif  // ifndef KERNELKIT_CODEMGR_PEF_H
