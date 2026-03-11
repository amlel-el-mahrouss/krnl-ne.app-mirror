// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <CFKit/Utils.h>
#include <KernelKit/DebugOutput.h>
#include <KernelKit/HeapMgr.h>
#include <KernelKit/PE32CodeMgr.h>
#include <KernelKit/ProcessScheduler.h>
#include <NeKit/Config.h>
#include <NeKit/KString.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/OwnPtr.h>

#define kPeStackSizeSymbol "__NESizeOfReserveStack"
#define kPeHeapSizeSymbol "__NESizeOfReserveHeap"
#define kPeNameSymbol "__NEProgramName"
#define kPeImageStart "__ImageStart"

namespace Kernel {

namespace Detail {
  /***********************************************************************************/
  /// @brief Get the PE32+ platform signature according to the compiled architecture.
  /***********************************************************************************/

  UInt32 ldr_get_platform_pe(void) {
#if defined(__NE_AMD64__)
    return kPEPlatformAMD64;
#elif defined(__NE_ARM64__)
    return kPEPlatformARM64;
#else
    return kPEPlatformInvalid;
#endif  // __32x0__ || __64x0__ || __x86_64__
  }
}  // namespace Detail

/***********************************************************************************/
/// @brief PE32+ loader constructor w/ blob.
/// @param blob file blob.
/***********************************************************************************/

PE32Loader::PE32Loader(const VoidPtr blob) : fCachedBlob(blob) {
  fBad = false;
}

/***********************************************************************************/
/// @brief PE32+ loader constructor.
/// @param path the filesystem path.
/***********************************************************************************/

PE32Loader::PE32Loader(const Char* path) : fCachedBlob(static_cast<VoidPtr>(nullptr)), fBad(false) {
  fFile.New(const_cast<Char*>(path), kRestrictRB);
  fPath = KStringBuilder::Construct(path).Leak();

  auto kPefHeader = "PE32_BLOB";
  fCachedBlob     = fFile->Read(kPefHeader, 0);

  if (fCachedBlob.HasError() || !fCachedBlob.Leak().Leak()) fBad = YES;
}

/***********************************************************************************/
/// @brief PE32+ destructor.
/***********************************************************************************/

PE32Loader::~PE32Loader() {
  if (fCachedBlob) {
    mm_free_ptr(fCachedBlob.Leak().Leak());
    fFile.Reset();
  }
}

/***********************************************************************************/
/// @brief Finds the section  according to its name.
/// @param name name of section.
/***********************************************************************************/

ErrorOr<VoidPtr> PE32Loader::FindSectionByName(const Char* name) {
  if (!fCachedBlob || fBad || !name) return ErrorOr<VoidPtr>{kErrorInvalidData};

  LDR_EXEC_HEADER_PTR header_ptr =
      CF::ldr_find_exec_header((const Char*) fCachedBlob.Leak().Leak());
  LDR_OPTIONAL_HEADER_PTR opt_header_ptr =
      CF::ldr_find_opt_exec_header((const Char*) fCachedBlob.Leak().Leak());

  if (!header_ptr || !opt_header_ptr) return ErrorOr<VoidPtr>{kErrorInvalidData};

#ifdef __NE_AMD64__
  if (header_ptr->Machine != kPeMachineAMD64 || header_ptr->Signature != kPeSignature) {
    return ErrorOr<VoidPtr>{kErrorInvalidData};
  }

#elif defined(__NE_ARM64__)
  if (header_ptr->Machine != kPeMachineARM64 || header_ptr->Signature != kPeSignature) {
    return ErrorOr<VoidPtr>{kErrorInvalidData};
  }
#endif  // __NE_AMD64__ || __NE_ARM64__

  if (header_ptr->NumberOfSections < 1) {
    return ErrorOr<VoidPtr>{kErrorInvalidData};
  }

#if !defined(__nekernel_allow_non_nekernel_pe)
  if (opt_header_ptr->Subsystem != kNeKernelPESubsystem) {
    return ErrorOr<VoidPtr>{kErrorInvalidData};
  }
#endif

  LDR_SECTION_HEADER_PTR secs =
      (LDR_SECTION_HEADER_PTR) (((Char*) opt_header_ptr) + header_ptr->SizeOfOptionalHeader);

  for (SizeT sectIndex = 0; sectIndex < header_ptr->NumberOfSections; ++sectIndex) {
    LDR_SECTION_HEADER_PTR sect = &secs[sectIndex];

    if (KStringBuilder::Equals(name, sect->Name)) {
      return ErrorOr<VoidPtr>(sect);
    }
  }

  return ErrorOr<VoidPtr>{kErrorInvalidData};
}

/***********************************************************************************/
/// @brief Finds the symbol according to it's name.
/// @param name name of symbol.
/// @param kind kind of symbol we want.
/***********************************************************************************/

ErrorOr<VoidPtr> PE32Loader::FindSymbol(const Char* name, Int32 kind) {
  if (!fCachedBlob || fBad || !name) return ErrorOr<VoidPtr>{kErrorInvalidData};

  auto section_name = "\0";

  switch (kind) {
    case kPETypeData:
      section_name = ".data";
      break;
    case kPETypeBSS:
      section_name = ".bss";
      break;
    case kPETypeText:
      section_name = ".text";
      break;
    default:
      return ErrorOr<VoidPtr>{kErrorInvalidData};
  }

  auto                    sec     = this->FindSectionByName(section_name);
  LDR_SECTION_HEADER_PTR* sec_ptr = (LDR_SECTION_HEADER_PTR*) sec.Leak().Leak();

  if (!sec_ptr || !*sec_ptr) return ErrorOr<VoidPtr>{kErrorInvalidData};

  LDR_OPTIONAL_HEADER_PTR opt_header_ptr =
      CF::ldr_find_opt_exec_header((const Char*) fCachedBlob.Leak().Leak());

  if (opt_header_ptr) {
    LDR_DATA_DIRECTORY_PTR data_dirs =
        (LDR_DATA_DIRECTORY_PTR) ((UInt8*) opt_header_ptr + sizeof(LDR_OPTIONAL_HEADER));

    LDR_DATA_DIRECTORY_PTR export_dir_entry = &data_dirs[0];

    if (export_dir_entry->VirtualAddress == 0 || export_dir_entry->Size == 0)
      return ErrorOr<VoidPtr>{kErrorInvalidData};

    LDR_EXPORT_DIRECTORY* export_dir =
        (LDR_EXPORT_DIRECTORY*) ((UIntPtr) fCachedBlob.Leak().Leak() +
                                 export_dir_entry->VirtualAddress);

    UInt32* name_table =
        (UInt32*) ((UIntPtr) fCachedBlob.Leak().Leak() + export_dir->AddressOfNames);
    UInt16* ordinal_table =
        (UInt16*) ((UIntPtr) fCachedBlob.Leak().Leak() + export_dir->AddressOfNameOrdinal);
    UInt32* function_table =
        (UInt32*) ((UIntPtr) fCachedBlob.Leak().Leak() + export_dir->AddressOfFunctions);

    for (UInt32 i = 0; i < export_dir->NumberOfNames; ++i) {
      const char* exported_name =
          (const char*) ((UIntPtr) fCachedBlob.Leak().Leak() + name_table[i]);

      if (KStringBuilder::Equals(exported_name, name)) {
        UInt16 ordinal = ordinal_table[i];
        UInt32 rva     = function_table[ordinal];

        VoidPtr symbol_addr = (VoidPtr) ((UIntPtr) fCachedBlob.Leak().Leak() + rva);

        return ErrorOr<VoidPtr>{symbol_addr};
      }
    }
  }

  return ErrorOr<VoidPtr>{kErrorInvalidData};
}

/// @brief Finds the executable entrypoint.
/// @return
ErrorOr<VoidPtr> PE32Loader::FindStart() {
  if (auto sym = this->FindSymbol(kPeImageStart, 0); sym) return sym;

  return ErrorOr<VoidPtr>(kErrorExecutable);
}

/// @brief Tells if the executable is loaded or not.
/// @return Whether it's not bad and is cached.
bool PE32Loader::IsLoaded() {
  return !fBad;
}

const Char* PE32Loader::Path() {
  return fPath.Leak().CData();
}

const Char* PE32Loader::AsString() {
#ifdef __32x0__
  return "32x0 PE";
#elif defined(__64x0__)
  return "64x0 PE";
#elif defined(__x86_64__)
  return "x86_64 PE";
#elif defined(__aarch64__)
  return "AARCH64 PE";
#elif defined(__powerpc64__)
  return "POWER64 PE";
#else
  return "???? PE";
#endif  // __32x0__ || __64x0__ || __x86_64__ || __powerpc64__
}

const Char* PE32Loader::MIME() {
  return kPeApplicationMime;
}

ErrorOr<VoidPtr> PE32Loader::GetBlob() {
  return ErrorOr<VoidPtr>{this->fCachedBlob.Leak().Leak()};
}

namespace Utils {
  ProcessID rtl_create_user_process(PE32Loader&                        exec,
                                    const UserProcess::ExecutableKind& process_kind) {
    if (!exec.IsLoaded()) return kSchedInvalidPID;

    ErrorOrAny errOrStart = exec.FindStart();

    if (errOrStart.Error() != kErrorSuccess) return kSchedInvalidPID;

    ErrorOrAny symname = exec.FindSymbol(kPeNameSymbol, 0);

    if (!symname.Leak().Leak())
      symname = ErrorOr<VoidPtr>{(VoidPtr) rt_alloc_string(kPeImageStart)};

    if (!symname.Leak().Leak()) return kSchedInvalidPID;

    ProcessID id =
        UserProcessScheduler::The().Spawn(reinterpret_cast<const Char*>(symname.Leak().Leak()),
                                          errOrStart.Leak().Leak(), exec.GetBlob().Leak().Leak());

    mm_free_ptr(symname.Leak().Leak());

    if (id != kSchedInvalidPID) {
      auto stacksym = exec.FindSymbol(kPeStackSizeSymbol, 0);

      if (!stacksym.Leak().Leak()) {
        stacksym = ErrorOr<VoidPtr>{(VoidPtr) new UIntPtr(kSchedMaxStackSz)};
      }

      if (!stacksym.Leak().Leak()) {
        UserProcessScheduler::The().Remove(id);
        mm_free_ptr(stacksym.Leak().Leak());
        return kSchedInvalidPID;
      }

      if ((*(volatile UIntPtr*) stacksym.Leak().Leak()) > kSchedMaxStackSz) {
        *(volatile UIntPtr*) stacksym.Leak().Leak() = kSchedMaxStackSz;
      }

      UserProcessScheduler::The().TheCurrentTeam().Leak().AsArray()[id].Kind = process_kind;
      UserProcessScheduler::The().TheCurrentTeam().Leak().AsArray()[id].StackSize =
          *(UIntPtr*) stacksym.Leak().Leak();

      mm_free_ptr(stacksym.Leak().Leak());
      stacksym.Leak().Leak() = nullptr;
    }

    return id;
  }
}  // namespace Utils

}  // namespace Kernel