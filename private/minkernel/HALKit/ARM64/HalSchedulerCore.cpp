// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <KernelKit/ProcessScheduler.h>

namespace Kernel {
/// @brief Wakes up thread.
/// Wakes up thread from the hang state.
Void mp_wakeup_thread(HAL::StackFrame* stack) {
  NE_UNUSED(stack);
}

/// @brief makes the thread sleep on a loop.
/// hooks and hangs thread to prevent code from executing.
Void mp_hang_thread(HAL::StackFrame* stack) {
  NE_UNUSED(stack);
}
}  // namespace Kernel
