// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef HALKIT_AP_H
#define HALKIT_AP_H

#include <NeKit/Config.h>

namespace Kernel {
typedef Int64 hal_ap_kind;

typedef struct HAL_HARDWARE_THREAD {
  Kernel::UIntPtr fStartAddress;
  Kernel::UIntPtr fStackPtr;
  Kernel::UIntPtr fFramePtr;
  Kernel::UInt8   fPrivileged : 1;
  Kernel::UInt32  fPageMemoryFlags;
  hal_ap_kind     fIdentNumber;
} HAL_HARDWARE_THREAD;

/// @brief Set PC to specific hart.
/// @param hart the hart
/// @param epc the pc.
/// @return
EXTERN_C Kernel::Void hal_set_pc_to_hart(HAL_HARDWARE_THREAD* hart, Kernel::VoidPtr epc);
}  // namespace Kernel

#endif
