// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <mindetect/DetectKit/DetectKit.h>

EXTERN_C Int32 OSDetectModuleMain(Kernel::HEL::BootInfoHeader* handover) {
  ::fw_init_efi((EfiSystemTable*) handover->f_FirmwareCustomTables[Kernel::HEL::kHandoverTableST]);

  if (handover->f_HardwareTables.f_MultiProcessingEnabled == false) return kEfiFail;

#ifdef __NE_ONLY_SMP__
    if (handover->f_NumberOfProcessors == 0) return kEfiFail;
#endif

  return kEfiOk;
}
