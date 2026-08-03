// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

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

#define kPe32BlobFork "__NEPe32Blob"

namespace Ne::Kernel {

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

  /// @brief COFF machine value this build can execute.
  UInt16 ldr_get_platform_pe_machine(void) {
#if defined(__NE_AMD64__)
    return kPeMachineAMD64;
#elif defined(__NE_ARM64__)
    return kPeMachineARM64;
#else
    return 0;
#endif
  }
}  // namespace Detail

/***********************************************************************************/
/// @brief PE32+ loader constructor w/ blob.
/// @param blob file blob.
/***********************************************************************************/

PE32Loader::PE32Loader(const VoidPtr blob, const SizeT len)
    : fCachedBlob(blob), fCachedBlobSz(len) {
  if (!blob || len < sizeof(DosHeader)) {
    fBad = YES;
    return;
  }

  auto header = CF::ldr_find_exec_header(reinterpret_cast<const Char*>(blob));
  auto opt    = CF::ldr_find_opt_exec_header(reinterpret_cast<const Char*>(blob));

  if (!header || !opt) {
    fBad = YES;
    return;
  }

  if (header->Machine != Detail::ldr_get_platform_pe_machine()) {
    fBad = YES;
    return;
  }

  if (opt->Subsystem != kNeKernelPESubsystem) {
    fBad = YES;
    return;
  }

  if (opt->SizeOfImage < 1 || opt->SizeOfImage > kPeMaxImageSz) {
    fBad = YES;
    return;
  }
}

/***********************************************************************************/
/// @brief PE32+ loader constructor.
/// @param path the filesystem path.
/***********************************************************************************/

PE32Loader::PE32Loader(const Char* path) : fCachedBlob(static_cast<VoidPtr>(nullptr)), fBad(false) {
  fFile.New(const_cast<Char*>(path), kRestrictRB);
  fPath = KStringBuilder::Construct(path).Leak();

  auto kPefHeader = kPe32BlobFork;
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
/***********************************************************************************/
/// @brief Instantiate the image and map it at its preferred base.
/// @return the kernel address of the image.
/***********************************************************************************/

ErrorOr<VoidPtr> PE32Loader::LoadImage() {
  if (fImage) return ErrorOr<VoidPtr>{fImage};

  if (fBad || !fCachedBlob.Leak().Leak() || !fCachedBlobSz)
    return ErrorOr<VoidPtr>{kErrorInvalidData};

  auto blob   = reinterpret_cast<Char*>(fCachedBlob.Leak().Leak());
  auto header = CF::ldr_find_exec_header(blob);
  auto opt    = CF::ldr_find_opt_exec_header(blob);

  if (!header || !opt || header->NumberOfSections < 1)
    return ErrorOr<VoidPtr>{kErrorInvalidData};

  auto image = new Char[opt->SizeOfImage];

  if (!image) return ErrorOr<VoidPtr>{kErrorHeapOutOfMemory};

  rt_set_memory(image, 0, opt->SizeOfImage);

  auto sect =
      reinterpret_cast<LDR_SECTION_HEADER_PTR>(((Char*) opt) + header->SizeOfOptionalHeader);

  for (SizeT i = 0UL; i < header->NumberOfSections; ++i) {
    auto sec = &sect[i];

    if (!sec->SizeOfRawData) continue;

    /// @note never a + b, both sums are free to wrap.
    if (sec->VirtualAddress > opt->SizeOfImage ||
        sec->SizeOfRawData > opt->SizeOfImage - sec->VirtualAddress ||
        sec->PointerToRawData > fCachedBlobSz ||
        sec->SizeOfRawData > fCachedBlobSz - sec->PointerToRawData) {
      delete[] image;

      return ErrorOr<VoidPtr>{kErrorInvalidData};
    }

    rt_copy_memory_safe((VoidPtr) (blob + sec->PointerToRawData),
                        (VoidPtr) (image + sec->VirtualAddress), sec->SizeOfRawData,
                        opt->SizeOfImage - sec->VirtualAddress);
  }

  SizeT pages = opt->SizeOfImage / kPageSize + ((opt->SizeOfImage % kPageSize) ? 1 : 0);

  for (SizeT i = 0UL; i < pages; ++i) {
    if (HAL::mm_map_page((VoidPtr) (opt->ImageBase + (i * kPageSize)),
                         (VoidPtr) (HAL::mm_get_page_addr(image) + (i * kPageSize)),
                         HAL::kMMFlagsPresent | HAL::kMMFlagsUser) != kErrorSuccess) {
      delete[] image;

      return ErrorOr<VoidPtr>{kErrorInvalidData};
    }
  }

  fImage = image;

  (Void)(kout << "PE32Loader: info: Mapped image at " << hex_number(opt->ImageBase) << kendl);

  return ErrorOr<VoidPtr>{fImage};
}

ErrorOr<VoidPtr> PE32Loader::FindStart() {
  auto image = this->LoadImage();

  if (!image.Leak().Leak()) return ErrorOr<VoidPtr>(kErrorExecutable);

  auto opt = CF::ldr_find_opt_exec_header(reinterpret_cast<Char*>(fCachedBlob.Leak().Leak()));

  if (!opt || !opt->AddressOfEntryPoint) return ErrorOr<VoidPtr>(kErrorExecutable);

  return ErrorOr<VoidPtr>{
      (VoidPtr) ((UIntPtr) image.Leak().Leak() + opt->AddressOfEntryPoint)};
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

ProcessID rtl_create_user_process(PE32Loader&                        exec,
                                    const UserProcess::ExecutableKind& process_kind) {
    if (!exec.IsLoaded()) return kCPSInvalidPID;

    ErrorOrAny errOrStart = exec.FindStart();

    if (errOrStart.Error() != kErrorSuccess) return kCPSInvalidPID;

    ErrorOrAny symname = exec.FindSymbol(kPeNameSymbol, 0);

    if (!symname.Leak().Leak())
      symname = ErrorOr<VoidPtr>{(VoidPtr) rt_alloc_string(kPeImageStart)};

    if (!symname.Leak().Leak()) return kCPSInvalidPID;

    ProcessID id =
        UserProcessScheduler::The().Spawn(reinterpret_cast<const Char*>(symname.Leak().Leak()),
                                          errOrStart.Leak().Leak(), exec.GetBlob().Leak().Leak());

    mm_free_ptr(symname.Leak().Leak());

    if (id != kCPSInvalidPID) {
      auto stacksym = exec.FindSymbol(kPeStackSizeSymbol, 0);

      if (!stacksym.Leak().Leak()) {
        stacksym = ErrorOr<VoidPtr>{(VoidPtr) new UIntPtr(kCPSMaxStackSz)};
      }

      if (!stacksym.Leak().Leak()) {
        UserProcessScheduler::The().Remove(id);
        mm_free_ptr(stacksym.Leak().Leak());
        return kCPSInvalidPID;
      }

      if ((*(volatile UIntPtr*) stacksym.Leak().Leak()) > kCPSMaxStackSz) {
        *(volatile UIntPtr*) stacksym.Leak().Leak() = kCPSMaxStackSz;
      }

      UserProcessScheduler::The().TheCurrentTeam().AsArray()[id].Kind = process_kind;
      UserProcessScheduler::The().TheCurrentTeam().AsArray()[id].StackSize =
          *(UIntPtr*) stacksym.Leak().Leak();

      mm_free_ptr(stacksym.Leak().Leak());
      stacksym.Leak().Leak() = nullptr;
    }

    return id;
  }

}  // namespace Ne::Kernel