// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <BootKit/BootKit.h>
#include <BootKit/BootThread.h>
#include <BootKit/HW/SATA.h>
#include <FirmwareKit/EFI.h>
#include <FirmwareKit/EFI/API.h>
#include <FirmwareKit/Handover.h>
#include <KernelKit/MSDOS.h>
#include <KernelKit/PE.h>
#include <KernelKit/PEF.h>
#include <NeKit/Macros.h>
#include <NeKit/Ref.h>
#include <modules/CoreGfx/CoreGfx.h>
#include <modules/CoreGfx/TextGfx.h>

EXTERN_C Int32 MinChkModuleMain(Kernel::HEL::BootInfoHeader* handover) {
  ::fw_init_efi((EfiSystemTable*) handover->f_FirmwareCustomTables[Kernel::HEL::kHandoverTableST]);

  return kEfiOk;
}
