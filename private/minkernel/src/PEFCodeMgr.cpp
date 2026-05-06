// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#include <ArchKit/ArchKit.h>
#include <KernelKit/DebugOutput.h>
#include <KernelKit/HeapMgr.h>
#include <KernelKit/PEFCodeMgr.h>
#include <KernelKit/ProcessScheduler.h>
#include <NeKit/Config.h>
#include <NeKit/KString.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/OwnPtr.h>
#include <NeKit/Utils.h>

/// @author Amlal El Mahrouss (amlal@nekernel.org)
/// @brief PEF backend for the Code Manager.
/// @file PEFCodeMgr.cc

/// @brief PEF stack size symbol.
#define kPefStackSizeSymbol "__PEFSizeOfReserveStack"
#define kPefHeapSizeSymbol "__PEFSizeOfReserveHeap"
#define kPefNameSymbol "__PEFProgramName"
#define kPefImageStart "__ImageStart"

namespace Kernel {

namespace Detail {
  /***********************************************************************************/
  /// @brief Get the PEF platform signature according to the compiled architecture.
  /***********************************************************************************/
  static UInt32 ldr_get_platform(void) {
#if defined(__NE_32X0__)
    return kPefArch32x0;
#elif defined(__NE_64X0__)
    return kPefArch64x0;
#elif defined(__NE_AMD64__)
    return kPefArchAMD64;
#elif defined(__NE_PPC64__)
    return kPefArchPowerPC;
#elif defined(__NE_ARM64__)
    return kPefArchARM64;
#else
    return kPefArchInvalid;
#endif  // __32x0__ || __64x0__ || __x86_64__
  }
}  // namespace Detail

/***********************************************************************************/
/// @brief PEF loader constructor w/ blob.
/// @param blob file blob.
/***********************************************************************************/
PEFLoader::PEFLoader(const VoidPtr blob) : fCachedBlob(blob) {
  if (fCachedBlob.Leak().Leak()) {
    this->fBad = YES;
    return;
  }

  PEFContainer* container = reinterpret_cast<PEFContainer*>(fCachedBlob.Leak().Leak());

  if (container->Abi == kPefAbi &&
      container->Count >=
          3) { /* if same ABI, AND: .text, .bss, .data (or at least similar) exists */
    if (container->Cpu == Detail::ldr_get_platform() && container->Magic[0] == kPefMagic[0] &&
        container->Magic[1] == kPefMagic[1] && container->Magic[2] == kPefMagic[2] &&
        container->Magic[3] == kPefMagic[3] && container->Magic[4] == kPefMagic[4]) {
      return;
    } else if (container->Magic[0] == kPefMagicFat[0] && container->Magic[1] == kPefMagicFat[1] &&
               container->Magic[2] == kPefMagicFat[2] && container->Magic[3] == kPefMagicFat[3] &&
               container->Magic[4] == kPefMagicFat[4]) {
      /// This is a fat binary. Treat it as such.
      this->fFatBinary = YES;
      return;
    }
  }

  kout << "PEFLoader: warning: Binary format error!\r";

  this->fFatBinary = NO;
  this->fBad       = YES;

  if (this->fCachedBlob) mm_free_ptr(this->fCachedBlob.Leak().Leak());
  this->fCachedBlob.Leak().Leak() = nullptr;
}

/***********************************************************************************/
/// @brief PEF loader constructor.
/// @param path the filesystem path.
/***********************************************************************************/
PEFLoader::PEFLoader(const Char* path) : fCachedBlob(nullptr), fFatBinary(false), fBad(false) {
  fFile.New(const_cast<Char*>(path), kRestrictRB);

  if (!fFile) {
    this->fBad = YES;
    return;
  }

  fPath = KStringBuilder::Construct(path).Leak();

  constexpr auto kPefHeader = "PEF_CONTAINER";

  /// @note zero here means that the FileMgr will read every container header inside the file.
  fCachedBlob = fFile->Read(kPefHeader, 0UL);

  if (!fCachedBlob.Leak().Leak()) {
    kout << "PEFLoader: warning: Binary format error!\r";
    this->fBad       = YES;
    this->fFatBinary = NO;

    return;
  }

  PEFContainer* container = reinterpret_cast<PEFContainer*>(fCachedBlob.Leak().Leak());

  if (container->Abi == kPefAbi &&
      container->Count >=
          3) { /* if same ABI, AND: .text, .bss, .data (or at least similar) exists */
    if (container->Cpu == Detail::ldr_get_platform() && container->Magic[0] == kPefMagic[0] &&
        container->Magic[1] == kPefMagic[1] && container->Magic[2] == kPefMagic[2] &&
        container->Magic[3] == kPefMagic[3] && container->Magic[4] == kPefMagic[4]) {
      return;
    } else if (container->Magic[0] == kPefMagicFat[0] && container->Magic[1] == kPefMagicFat[1] &&
               container->Magic[2] == kPefMagicFat[2] && container->Magic[3] == kPefMagicFat[3] &&
               container->Magic[4] == kPefMagicFat[4]) {
      /// This is a fat binary, treat it as such.
      this->fFatBinary = YES;
      return;
    }
  }

  kout << "PEFLoader: warning: Binary format error!\r";

  this->fFatBinary = NO;
  this->fBad       = YES;

  if (this->fCachedBlob.Leak().Leak()) mm_free_ptr(this->fCachedBlob.Leak().Leak());
  this->fCachedBlob.Leak().Leak() = nullptr;
}

/***********************************************************************************/
/// @brief PEF destructor.
/***********************************************************************************/
PEFLoader::~PEFLoader() {
  if (fCachedBlob.Leak().Leak()) {
    mm_free_ptr(fCachedBlob.Leak().Leak());
  }
}

/***********************************************************************************/
/// @brief Finds the symbol according to it's name.
/// @param name name of symbol.
/// @param kind kind of symbol we want.
/***********************************************************************************/
ErrorOr<VoidPtr> PEFLoader::FindSymbol(const Char* name, Int32 kind) {
  if (!fCachedBlob.Leak().Leak() || fBad || !name) return ErrorOr<VoidPtr>{kErrorInvalidData};

  auto blob = fFile->Read(name, sizeof(PEFCommandHeader));

  if (!blob) return ErrorOr<VoidPtr>{kErrorInvalidData};

  PEFContainer* container = reinterpret_cast<PEFContainer*>(fCachedBlob.Leak().Leak());

  if (!container) return ErrorOr<VoidPtr>{kErrorInvalidData};

  PEFCommandHeader* command_header = reinterpret_cast<PEFCommandHeader*>(blob.Leak().Leak());

  if (command_header->VMSize < 1 || command_header->VMAddress == 0)
    return ErrorOr<VoidPtr>{kErrorInvalidData};

  /// fat binary check.
  if (command_header->Cpu != container->Cpu && !this->fFatBinary)
    return ErrorOr<VoidPtr>{kErrorInvalidData};

  const auto  kMangleCharacter  = '$';
  const Char* kContainerKinds[] = {".code64", ".data64", ".zero64", nullptr};

  ErrorOr<KString> error_or_symbol;

  switch (kind) {
    case kPefCode: {
      error_or_symbol = KStringBuilder::Construct(kContainerKinds[0]);  // code symbol.
      break;
    }
    case kPefData: {
      error_or_symbol = KStringBuilder::Construct(kContainerKinds[1]);  // data symbol.
      break;
    }
    case kPefZero: {
      error_or_symbol = KStringBuilder::Construct(kContainerKinds[2]);  // block starting symbol.
      break;
    }
    default:
      return ErrorOr<VoidPtr>{kErrorInvalidData};
      // prevent that from the kernel's mode perspective, let that happen if it
      // were a user process.
  }

  Char* unconst_symbol = const_cast<Char*>(name);

  for (SizeT i = 0UL; i < rt_string_len(unconst_symbol, kPefNameLen); ++i) {
    if (rt_is_space(unconst_symbol[i])) {
      unconst_symbol[i] = kMangleCharacter;
    }
  }

  error_or_symbol.Leak().Leak() += name;

  if (KStringBuilder::Equals(command_header->Name, error_or_symbol.Leak().Leak().CData())) {
    if (command_header->Kind == kind) {
      if (command_header->Cpu != Detail::ldr_get_platform()) {
        if (!this->fFatBinary) {
          mm_free_ptr(blob.Leak().Leak());
          blob.Leak().Leak() = nullptr;

          return ErrorOr<VoidPtr>{kErrorInvalidData};
        }
      }

      Char* container_blob_value = new Char[command_header->VMSize];

      rt_copy_memory_safe((VoidPtr) ((Char*) blob.Leak().Leak() + sizeof(PEFCommandHeader)),
                          container_blob_value, command_header->VMSize, command_header->VMSize);

      mm_free_ptr(blob.Leak().Leak());

      kout << "PEFLoader: info: Load stub: " << command_header->Name << "!\r";

      Int32 ret         = 0;
      SizeT pages_count = (command_header->VMSize + kPageSize - 1) / kPageSize;

      for (SizeT i_vm{}; i_vm < pages_count; ++i_vm) {
        ret = HAL::mm_map_page(
            (VoidPtr) (command_header->VMAddress + (i_vm * kPageSize)),
            (VoidPtr) (HAL::mm_get_page_addr(container_blob_value) + (i_vm * kPageSize)),
            HAL::kMMFlagsPresent | HAL::kMMFlagsUser);

        if (ret != kErrorSuccess) {
          /// We set the VMAddress to nullptr, if the mapping fail here.
          for (SizeT i_vm_unmap{}; i_vm_unmap < i_vm; ++i_vm_unmap) {
            ret = HAL::mm_map_page((VoidPtr) (command_header->VMAddress + (i_vm * kPageSize)),
                                   nullptr, HAL::kMMFlagsPresent | HAL::kMMFlagsUser);
          }

          delete[] container_blob_value;
          container_blob_value = nullptr;

          return ErrorOr<VoidPtr>{kErrorInvalidData};
        }
      }

      return ErrorOr<VoidPtr>{container_blob_value};
    }
  }

  mm_free_ptr(blob.Leak().Leak());
  return ErrorOr<VoidPtr>{kErrorInvalidData};
}

/// @brief Finds the executable entrypoint.
/// @return
ErrorOr<VoidPtr> PEFLoader::FindStart() {
  if (auto sym = this->FindSymbol(kPefStart, kPefCode); sym) return sym;

  return ErrorOr<VoidPtr>(kErrorExecutable);
}

/// @brief Tells if the executable is loaded or not.
/// @return Whether it's not bad and is cached.
bool PEFLoader::IsLoaded() {
  return fBad == false;
}

const Char* PEFLoader::Path() {
  return fPath.Leak().CData();
}

const Char* PEFLoader::AsString() {
#ifdef __32x0__
  return "32x0 PEF";
#elif defined(__64x0__)
  return "64x0 PEF";
#elif defined(__x86_64__)
  return "x86_64 PEF";
#elif defined(__aarch64__)
  return "AARCH64 PEF";
#elif defined(__powerpc64__)
  return "POWER64 PEF";
#else
  return "???? PEF";
#endif  // __32x0__ || __64x0__ || __x86_64__ || __powerpc64__
}

const Char* PEFLoader::MIME() {
  return kPefApplicationMime;
}

ErrorOr<VoidPtr> PEFLoader::GetBlob() {
  return ErrorOr<VoidPtr>{this->fCachedBlob};
}

ProcessID rtl_create_user_process(PEFLoader&                         exec,
                                  const UserProcess::ExecutableKind& process_kind) {
  if (!exec.IsLoaded()) return kCPSInvalidPID;

  auto errOrStart = exec.FindStart();

  if (errOrStart.Error() != kErrorSuccess) return kCPSInvalidPID;

  auto symname = exec.FindSymbol(kPefNameSymbol, kPefCode);

  if (!symname.Leak().Leak()) symname = ErrorOr<VoidPtr>{(VoidPtr) rt_alloc_string(kPefImageStart)};

  if (!symname.Leak().Leak()) return kCPSInvalidPID;

  ProcessID id =
      UserProcessScheduler::The().Spawn(reinterpret_cast<const Char*>(symname.Leak().Leak()),
                                        errOrStart.Leak().Leak(), exec.GetBlob().Leak().Leak());

  if (symname.Leak().Leak()) mm_free_ptr(symname.Leak().Leak());

  if (id != kCPSInvalidPID) {
    auto stacksym = exec.FindSymbol(kPefStackSizeSymbol, kPefData);

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

    UserProcessScheduler::The().TheCurrentTeam().Leak().AsArray()[id].Kind = process_kind;
    UserProcessScheduler::The().TheCurrentTeam().Leak().AsArray()[id].StackSize =
        *(UIntPtr*) stacksym.Leak().Leak();

    mm_free_ptr(stacksym.Leak().Leak());
  }

  return id;
}

}  // namespace Kernel
