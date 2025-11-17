/* -------------------------------------------

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

------------------------------------------- */

#ifndef _INC_CODE_MANAGER_PEF_H_
#define _INC_CODE_MANAGER_PEF_H_

/// @file PEFCodeMgr.h
/// @brief PEF Code Manager header file.
/// @author Amlal El Mahrouss (amlal@nekernel.org)

#include <KernelKit/FileMgr.h>
#include <KernelKit/PEF.h>
#include <NeKit/ErrorOr.h>
#include <NeKit/KString.h>

#ifndef INC_PROCESS_SCHEDULER_H
#include <KernelKit/ProcessScheduler.h>
#endif

#define kPefApplicationMime "application/vnd-nekernel-executable"

namespace Kernel {
///
/// \name PEFLoader
/// \brief PEF loader class.
///
class PEFLoader : public ILoader {
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
  bool IsLoaded() noexcept;

 private:
#ifdef __FSKIT_INCLUDES_NEFS__
  OwnPtr<FileStream<Char, NeFileSystemMgr>> fFile;
#elif defined(__FSKIT_INCLUDES_OPENHEFS__)
  OwnPtr<FileStream<Char, HeFileSystemMgr>> fFile;
#else
  OwnPtr<FileStream<Char>> fFile;
#endif  // __FSKIT_INCLUDES_NEFS__

  Ref<KString> fPath;
  VoidPtr      fCachedBlob;
  BOOL         fFatBinary{};
  BOOL         fBad{};
};

namespace Utils {
  ProcessID rtl_create_user_process(PEFLoader& exec, const Int32& procKind) noexcept;
}  // namespace Utils
}  // namespace Kernel

#endif  // ifndef _INC_CODE_MANAGER_PEF_H_
