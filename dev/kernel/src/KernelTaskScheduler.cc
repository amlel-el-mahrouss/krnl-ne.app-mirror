/* -------------------------------------------

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

  FILE: KernelTaskScheduler.cc
  PURPOSE: Kernel Task scheduler.

------------------------------------------- */

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

Bool KernelTaskHelper::CanBeScheduled(const KERNEL_TASK& task) {
  return task.Kid > 0 && task.Image.HasCode();
}
}  // namespace Kernel