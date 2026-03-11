// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <ArchKit/ArchKit.h>
#include <CFKit/Property.h>
#include <FirmwareKit/Handover.h>
#include <HALKit/ARM64/ApplicationProcessor.h>
#include <HALKit/ARM64/Processor.h>
#include <KernelKit/CodeMgr.h>
#include <KernelKit/FileMgr.h>
#include <KernelKit/HardwareThreadScheduler.h>
#include <KernelKit/HeapMgr.h>
#include <KernelKit/PEFCodeMgr.h>
#include <KernelKit/ProcessScheduler.h>
#include <NeKit/Json.h>
#include <NetworkKit/IPC.h>
#include <modules/ACPI/ACPIFactoryInterface.h>
#include <modules/CoreGfx/CoreGfx.h>

#ifndef __NE_MODULAR_KERNEL_COMPONENTS__
EXTERN_C void hal_init_platform(Kernel::HEL::BootInfoHeader* handover_hdr) {
  using namespace Kernel;

  /************************************************** */
  /*     INITIALIZE AND VALIDATE HEADER.              */
  /************************************************** */

  kHandoverHeader = handover_hdr;

  if (kHandoverHeader->f_Magic != kHandoverMagic &&
      kHandoverHeader->f_Version != kHandoverVersion) {
    return;
  }

#ifdef __NE_ARM64_EFI__
  fw_init_efi((EfiSystemTable*) handover_hdr->f_FirmwareCustomTables[1]);

  Boot::ExitBootServices(handover_hdr->f_HardwareTables.f_ImageKey,
                         handover_hdr->f_HardwareTables.f_ImageHandle);
#endif

  FB::cg_clear_video();

  /************************************** */
  /*     INITIALIZE BIT MAP.              */
  /************************************** */

  kBitMapCursor     = 0UL;
  kKernelBitMpSize  = kHandoverHeader->f_BitMapSize;
  kKernelBitMpStart = reinterpret_cast<Kernel::VoidPtr>(
      reinterpret_cast<Kernel::UIntPtr>(kHandoverHeader->f_BitMapStart));

  /// @note do initialize the interrupts after it.

  Kernel::mp_init_cores();

  while (YES);
}
#endif
