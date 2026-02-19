// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#include <HALKit/ARM64/Processor.h>
#include <KernelKit/ProcessScheduler.h>

namespace Kernel {
/***********************************************************************************/
/// @brief Unimplemented function (crashes by default)
/// @param process The process handle.
/***********************************************************************************/

EXTERN_C Void __ne_pure_call(UserProcess* process) {
  if (process) process->Crash();
}

/***********************************************************************************/
/// @brief Validate user stack.
/// @param stack_ptr the frame pointer.
/***********************************************************************************/

EXTERN_C Bool hal_check_task(HAL::StackFramePtr stack_ptr) {
  if (!stack_ptr) return No;

  return stack_ptr->SP != 0 && stack_ptr->IP != 0;
}
}  // namespace Kernel
