// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#include <BootKit/BootKit.h>
#include <BootKit/BootThread.h>
#include <FirmwareKit/EFI/API.h>

EXTERN_C Int32 MemoryTestModuleMain(Kernel::HEL::BootInfoHeader* handover) {
  ::fw_init_efi(static_cast<EfiSystemTable*>(handover->f_FirmwareCustomTables[Kernel::HEL::kHandoverTableST]));

  Boot::BootTextWriter writer;
  writer.Write("MemoryTest: Testing Memory...\r");

  constexpr auto kTestValue = 0x20000000L;

  Int32* mem = (Int32*) kTestValue;
  if (!mem) return kEfiFail;

  auto prev = *mem;
  *mem      = 42;
  if (*mem != 42) {
    return kEfiFail;
  }

  *mem = prev;

  return kEfiOk;
}
