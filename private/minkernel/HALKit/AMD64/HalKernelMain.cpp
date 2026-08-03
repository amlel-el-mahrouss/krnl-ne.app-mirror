// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <ArchKit/ArchKit.h>
#include <CFKit/Property.h>
#include <FirmwareKit/EFI/API.h>
#include <FirmwareKit/EFI/EFI.h>
#include <KernelKit/CodeMgr.h>
#include <KernelKit/HardwareThreadScheduler.h>
#include <KernelKit/PEFCodeMgr.h>
#include <KernelKit/ProcessScheduler.h>
#include <KernelKit/Timer.h>
#include <NetworkKit/IPC.h>
#include <StorageKit/AHCI.h>
#include <modules/ACPI/ACPIFactoryInterface.h>
#include <modules/BenchKit/HWChronometer.h>
#include <modules/CoreGfx/TextGfx.h>

#ifndef __NE_MODULAR_KERNEL_COMPONENTS__
EXTERN_C Ne::Kernel::VoidPtr kInterruptVectorTable[];

/// @brief Ne::Kernel init function.
/// @param handover_hdr Handover boot header.
EXTERN_C Ne::Kernel::Int32 hal_init_platform(Ne::Kernel::HEL::BootInfoHeader* handover_hdr) {
  using namespace Ne::Kernel;

  if (handover_hdr->f_Magic != kHandoverMagic || handover_hdr->f_Version != kHandoverVersion) {
    return kEfiFail;
  }

  HAL::rt_sti();

  ::fw_init_efi(static_cast<EfiSystemTable*>(handover_hdr->f_FirmwareCustomTables[Ne::Kernel::HEL::kHandoverTableST]));

  Boot::ExitBootServices(handover_hdr->f_HardwareTables.f_ImageKey,
                         handover_hdr->f_HardwareTables.f_ImageHandle);

  kHandoverHeader = handover_hdr;

  kKernelVM = kHandoverHeader->f_PageStart;

  if (!kKernelVM) {
    MUST_PASS(kKernelVM);
    return kEfiFail;
  }

  hal_write_cr3(kKernelVM);

  /************************************** */
  /*     INITIALIZE BIT MAP.              */
  /************************************** */

  kBitMapCursor    = 0UL;
  kKernelBitMpSize = kHandoverHeader->f_BitMapSize;
  kKernelBitMpStart =
      reinterpret_cast<VoidPtr>(reinterpret_cast<UIntPtr>(kHandoverHeader->f_BitMapStart));

  /************************************** */
  /*     INITIALIZE GDT AND SEGMENTS. */
  /************************************** */

  STATIC CONST auto kGDTEntriesCount = 8;

  STATIC HAL::Detail::NE_TSS kKernelTSS{};

  if (!kHandoverHeader->f_StackTop) {
    ke_stop(RUNTIME_CHECK_BOOTSTRAP, "No ring 0 stack in handover.");
  }

  kKernelTSS.fRsp0 = (UInt64) kHandoverHeader->f_StackTop;
  kKernelTSS.fIopb = sizeof(HAL::Detail::NE_TSS);

  /* The GDT, mostly descriptors for user and kernel segments. */
  STATIC HAL::Detail::NE_GDT_ENTRY ALIGN(0x08) kGDTArray[kGDTEntriesCount] = {
      {.fLimitLow   = 0,
       .fBaseLow    = 0,
       .fBaseMid    = 0,
       .fAccessByte = 0x00,
       .fFlags      = 0x00,
       .fBaseHigh   = 0},  // Null entry
      {.fLimitLow   = 0x0,
       .fBaseLow    = 0,
       .fBaseMid    = 0,
       .fAccessByte = 0x9A,
       .fFlags      = 0xAF,
       .fBaseHigh   = 0},  // Ne::Kernel code
      {.fLimitLow   = 0x0,
       .fBaseLow    = 0,
       .fBaseMid    = 0,
       .fAccessByte = 0x92,
       .fFlags      = 0xCF,
       .fBaseHigh   = 0},  // Ne::Kernel data
      {},                // TSS data low
      {},                // TSS data high
      {.fLimitLow   = 0x0,
       .fBaseLow    = 0,
       .fBaseMid    = 0,
       .fAccessByte = 0xFA,
       .fFlags      = 0xAF,
       .fBaseHigh   = 0},  // User code
      {.fLimitLow   = 0x0,
       .fBaseLow    = 0,
       .fBaseMid    = 0,
       .fAccessByte = 0xF2,
       .fFlags      = 0xCF,
       .fBaseHigh   = 0},  // User data
  };

  kGDTArray[3].fLimitLow   = sizeof(HAL::Detail::NE_TSS) - 1;
  kGDTArray[3].fBaseLow    = ((UIntPtr) &kKernelTSS) & 0xFFFF;
  kGDTArray[3].fBaseMid    = (((UIntPtr) &kKernelTSS) >> 16) & 0xFF;
  kGDTArray[3].fAccessByte = 0x89;  // Present, type 9 = 64-bit available TSS
  kGDTArray[3].fFlags      = 0x20 | ((((UIntPtr) &kKernelTSS) >> 24) & 0x0F);
  kGDTArray[3].fBaseHigh   = (((UIntPtr) &kKernelTSS) >> 24) & 0xFF;

  kGDTArray[4].fLimitLow   = ((UIntPtr) &kKernelTSS >> 32) & 0xFFFF;
  kGDTArray[4].fBaseLow    = 0;
  kGDTArray[4].fBaseMid    = 0;
  kGDTArray[4].fAccessByte = 0;
  kGDTArray[4].fFlags      = 0;
  kGDTArray[4].fBaseHigh   = 0;

  FB::cg_clear_video();

  // Load memory descriptors.
  HAL::Register64 gdt_reg;

  gdt_reg.Base  = reinterpret_cast<UIntPtr>(kGDTArray);
  gdt_reg.Limit = (sizeof(HAL::Detail::NE_GDT_ENTRY) * kGDTEntriesCount) - 1;

  //! GDT will load hal_read_init after it successfully loads the segments.
  HAL::GDTLoader gdt_loader;
  gdt_loader.Load(gdt_reg);

  return kEfiFail;
}

EXTERN_C BOOL rtl_init_nic_rtl8139();

/// @brief Liveness probe, returns the handover magic to its caller.
STATIC Ne::Kernel::VoidPtr ke_ping(Ne::Kernel::VoidPtr arg) {
  NE_UNUSED(arg);
  return (Ne::Kernel::VoidPtr) kHandoverMagic;
}

EXTERN_C Ne::Kernel::Void hal_real_init(Ne::Kernel::Void) {
  HAL::mp_init_cores(kHandoverHeader->f_HardwareTables.f_VendorPtr);

  HAL::Register64 idt_reg;
  idt_reg.Base = reinterpret_cast<UIntPtr>(kInterruptVectorTable);

  HAL::IDTLoader idt_loader;
  idt_loader.Load(idt_reg);

  user_init_globals(kHandoverHeader->f_RecoverMode);

  ke_install_syscall("ke_ping", ke_ping);

#ifdef __FSKIT_INCLUDES_OPENHEFS__
  OpenHeFS::fs_init_openhefs();
  HeFileSystemMgr::Mount(new HeFileSystemMgr());
#endif

#ifdef __FSKIT_INCLUDES_NEFS__
  NeFS::fs_init_nefs();
  NeFileSystemMgr::Mount(new NeFileSystemMgr());
#endif

  UserProcessScheduler::The().SwitchTeam(kRTUserTeam);

  if (kHandoverHeader->f_SCIImage && kHandoverHeader->f_SCIImageSz) {
    PE32Loader ldr(kHandoverHeader->f_SCIImage, kHandoverHeader->f_SCIImageSz);

    if (ldr.IsLoaded() &&
        rtl_create_user_process(ldr, UserProcess::ExecutableKind::kExecutableKind) !=
            kCPSInvalidPID) {
      (Void)(kout << "hal_real_init: Spawned the launch host.\r");
    } else {
      (Void)(kout << "hal_real_init: warning: Launch host did not spawn.\r");
    }
  } else {
    (Void)(kout << "hal_real_init: warning: No launch host in handover.\r");
  }

  UserProcessScheduler::The().SwitchTeam(kMidUserTeam);

#ifdef __HALKIT_INCLUDES_BNID__
  rtl_init_nic_rtl8139();
#endif

  while (YES);
}
#endif  // ifndef __NE_MODULAR_KERNEL_COMPONENTS__
