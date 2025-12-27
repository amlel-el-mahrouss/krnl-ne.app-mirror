// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#include <KernelKit/KernelTaskScheduler.h>

/***********************************************************************************/
/// @file KernelTaskScheduler.cc
/// @brief Kernel Task scheduler.
/// @author Amlal El Mahrouss (amlal@nekernel.org)
/***********************************************************************************/

namespace Kernel {
EXTERN_C Void hal_switch_kernel_task(HAL::StackFramePtr frame, KID kid);

Bool KernelTaskHelper::Add(HAL::StackFramePtr frame_ptr, KID new_kid) {
  NE_UNUSED(frame_ptr);
  NE_UNUSED(new_kid);

  return NO;
}

Bool KernelTaskHelper::Remove(const KID kid) {
  NE_UNUSED(kid);

  return NO;
}

Bool KernelTaskHelper::CanBeScheduled(const KernelTask& task) {
  return task.Kid > 0 && task.Image.HasCode();
}
}  // namespace Kernel