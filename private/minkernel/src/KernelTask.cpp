// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#include <KernelKit/CodeMgr.h>
#include <KernelKit/KernelTask.h>

/***********************************************************************************/
/// @file KernelTaskScheduler.cc
/// @brief Kernel Task scheduler.
/// @author Amlal El Mahrouss (amlal@nekernel.org)
/***********************************************************************************/

namespace Kernel {

/// @internal @brief Calls the DDK stub to initialize the stack_frame of the driver.
EXTERN_C Int32 kt_kernel_task_start(HAL::StackFramePtr stack_frame, VoidPtr code) {
  MUST_PASS(stack_frame && code);

  if (!stack_frame || !code) ke_stop(RUNTIME_CHECK_BAD_BEHAVIOR, "Kernel task arguments are invalid");
  ((rtl_kstart_kind) (code))(stack_frame);
  if (!stack_frame->R8) ke_stop(RUNTIME_CHECK_BAD_BEHAVIOR, "Kernel task failed to run");

  return stack_frame->R8;
}

Bool KernelTaskHelper::Start(KernelTask& task_ptr, const KID& kid) {
  if (!kid) return NO;

  task_ptr.Kid = kid;

  auto ret = kt_kernel_task_start(task_ptr.StackFrame, task_ptr.Image.LeakImage().Leak().Leak());
  if (ret != kErrorSuccess) return FALSE;
  
  return TRUE;
}

Bool KernelTaskHelper::CanBeStarted(const KernelTask& task) {
  return task.StackSize > 0 && task.Image.HasCode() && task.Image.HasImage();
}

}  // namespace Kernel
