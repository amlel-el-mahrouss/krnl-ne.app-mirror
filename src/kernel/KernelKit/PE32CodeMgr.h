/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

  File: PE32CodeMgr.h
  Purpose: PE32+ Code Mgr and Dylib mgr.

  Revision History:

  12/02/24: Added file (amlel)

======================================== */

#pragma once

////////////////////////////////////////////////////

// LAST REV: Mon Feb 12 13:52:01 CET 2024

////////////////////////////////////////////////////

#include <KernelKit/FileMgr.h>
#include <KernelKit/ILoader.h>
#include <KernelKit/PE.h>
#include <NeKit/ErrorOr.h>
#include <NeKit/KString.h>

#ifndef __KERNEL_KIT_USER_PROCESS_SCHEDULER_H__
#include <KernelKit/ProcessScheduler.h>
#endif

#define kPeStackSizeSymbol "__NESizeOfReserveStack"
#define kPeHeapSizeSymbol "__NESizeOfReserveHeap"
#define kPeNameSymbol "__NEProgramName"

#define kPeApplicationMime "application/vnd-portable-executable"

#define kPeImageStart "__ImageStart"

namespace Kernel {
///
/// \name PE32Loader
/// \brief PE32+ loader class.
///
class PE32Loader : public ILoader {
 private:
  explicit PE32Loader() = delete;

 public:
  explicit PE32Loader(const VoidPtr blob);
  explicit PE32Loader(const Char* path);
  ~PE32Loader() override;

 public:
  NE_COPY_DEFAULT(PE32Loader)

 public:
  const Char* Path() override;
  const Char* AsString() override;
  const Char* MIME() override;

 public:
  ErrorOr<VoidPtr> FindStart() override;
  ErrorOr<VoidPtr> FindSectionByName(const Char* name);
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
  VoidPtr      fCachedBlob;
  BOOL         fBad;
};

enum { kPEPlatformInvalid, kPEPlatformAMD64 = 100, kPEPlatformARM64 };
enum { kPETypeInvalid, kPETypeText = 100, kPETypeData, kPETypeBSS };

typedef LDR_SECTION_HEADER PE_SECTION_INFO;

ProcessID rtl_create_user_process(PE32Loader& exec, const Int32& process_kind);
}  // namespace Kernel