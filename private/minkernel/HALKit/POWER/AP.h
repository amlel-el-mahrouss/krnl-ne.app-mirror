// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef HALKIT_AP_H
#define HALKIT_AP_H

#include <NeKit/Config.h>

namespace Ne::Kernel {
struct HAL_HARDWARE_THREAD;

/// @brief hardware thread indentification type.
typedef Ne::Kernel::Int32 hal_ap_kind;

/// @brief Hardware thread information structure.
typedef struct HAL_HARDWARE_THREAD {
  Ne::Kernel::UIntPtr fStartAddress;
  Ne::Kernel::UIntPtr fStackPtr;
  Ne::Kernel::UIntPtr fFramePtr;
  Ne::Kernel::UInt8   fPrivileged : 1;
  Ne::Kernel::UInt32  fPageMemoryFlags;
  hal_ap_kind     fIdentNumber;
} HAL_HARDWARE_THREAD;

/// @brief Set PC to specific hart.
/// @param hart the hart
/// @param epc the pc.
/// @return
EXTERN_C Ne::Kernel::Void hal_set_pc_to_hart(HAL_HARDWARE_THREAD* hart, Ne::Kernel::VoidPtr epc);
}  // namespace Ne::Kernel

#endif
