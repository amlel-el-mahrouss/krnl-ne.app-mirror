// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <KernelKit/KernelTask.h>
#include <KernelKit/CodeMgr.h>

/***********************************************************************************/
/// @file KernelTaskScheduler.cc
/// @brief Kernel Task scheduler.
/// @author Amlal El Mahrouss (amlal@nekernel.org)
/***********************************************************************************/

namespace Kernel {
  
/// @internal @brief Calls the DDK stub to initialize the stack_frame of the driver.
EXTERN_C Int32 kt_kernel_task_start(HAL::StackFramePtr stack_frame, VoidPtr code) {
  MUST_PASS(stack_frame && code);
  ((rtl_kstart_kind)(code))(stack_frame);

  return stack_frame->R8;
}
  
Bool KernelTaskHelper::Start(KernelTask& task_ptr, const KID& kid) {
  if (!kid) return NO;

  auto ret = kt_kernel_task_start(task_ptr.StackFrame, task_ptr.Image.LeakImage().Leak().Leak());
  return ret == kErrorSuccess;
}

Bool KernelTaskHelper::CanBeStarted(const KernelTask& task) {
  return task.StackSize > 0 && task.Image.HasCode();
}

}  // namespace Kernel
