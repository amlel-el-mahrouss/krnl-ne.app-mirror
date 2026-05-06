// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef __EFI_API__
#define __EFI_API__

#include <FirmwareKit/EFI/EFI.h>
#include <FirmwareKit/Handover.h>
#include <KernelKit/MSDOS.h>
#include <KernelKit/PE.h>

#ifdef __BOOTZ__
// forward decl.
class BootTextWriter;

#define __BOOTKIT_NO_INCLUDE__ 1

#include <BootKit/BootKit.h>
#include <modules/CoreGfx/CoreGfx.h>
#endif  // ifdef __BOOTZ__

inline EfiSystemTable*  ST = nullptr;
inline EfiBootServices* BS = nullptr;

EXTERN_C void rt_cli();
EXTERN_C void rt_halt();

namespace Boot {
/// @brief Halt and clear interrut flag on x86.
/// @return
inline Void Stop() {
  while (YES) {
    rt_cli();
    rt_halt();
  }
}

/**
@brief Exit EFI API to let the OS load correctly.
Bascially frees everything we have in the EFI side.
*/
inline Void ExitBootServices(UInt64 MapKey, EfiHandlePtr ImageHandle) {
  if (!ST) return;

  ST->BootServices->ExitBootServices(ImageHandle, MapKey);
}

inline UInt32 Platform() {
  return kPeMachineAMD64;
}

/***
 * @brief Throw an error, stop execution as well.
 * @param ErrorCode error code to be print.
 * @param Reason reason to be print.
 */
inline void ThrowError(const EfiCharType* ErrorCode, const EfiCharType* Reason) {
  ST->ConOut->OutputString(ST->ConOut, L"\r*** STOP ***\r");

  ST->ConOut->OutputString(ST->ConOut, L"*** ERROR: ");
  ST->ConOut->OutputString(ST->ConOut, ErrorCode);

  ST->ConOut->OutputString(ST->ConOut, L" ***\r *** REASON: ");
  ST->ConOut->OutputString(ST->ConOut, Reason);

  ST->ConOut->OutputString(ST->ConOut, L" ***\r");

  Boot::Stop();
}
}  // namespace Boot

inline void fw_init_efi(EfiSystemTable* SystemTable) {
  ST = SystemTable;
  BS = ST->BootServices;
}

#ifdef __BOOTZ__

#include <BootKit/Platform.h>

#endif  // ifdef __BOOTZ__

#endif /* ifndef __EFI_API__ */
