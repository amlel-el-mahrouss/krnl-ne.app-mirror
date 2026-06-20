// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <HALKit/POWER/AP.h>
#include <HALKit/POWER/Processor.h>
#include <KernelKit/DebugOutput.h>

/// @note This part of the HAL needs an owner.

namespace Kernel::Detail {
STATIC void mp_hang_fn(void) {
  while (YES);
}
}  // namespace Kernel::Detail

namespace Kernel {
/// @brief wakes up thread.
/// wakes up thread from hang.
void mp_wakeup_thread(HAL::StackFramePtr stack) {
  if (!stack) return;

  MUST_PASS(stack->R15 > 0);
  MUST_PASS(stack->IP > 0);

  hal_set_pc_to_hart(reinterpret_cast<HAL_HARDWARE_THREAD*>(stack->R15),
                     reinterpret_cast<VoidPtr>(stack->IP));
}

/// @brief makes thread sleep.
/// hooks and hangs thread to prevent code from executing.
void mp_hang_thread(HAL::StackFramePtr stack) {
  if (!stack) return;

  MUST_PASS(stack->R15 > 0);

  hal_set_pc_to_hart(reinterpret_cast<HAL_HARDWARE_THREAD*>(stack->R15),
                     reinterpret_cast<VoidPtr>(Kernel::Detail::mp_hang_fn));
}
}  // namespace Kernel
