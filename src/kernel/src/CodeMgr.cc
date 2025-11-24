/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#include <KernelKit/CodeMgr.h>
#include <KernelKit/ProcessScheduler.h>
#include <NeKit/Utils.h>

namespace Kernel {

/// @brief Executes a new process from a function. Kernel code only.
/// @note This sets up a new stack, anything on the main function that calls the Kernel will not be
/// accessible.
/// @param main the start of the process.
/// @return The team's process id.
BOOL rtl_create_kernel_task(HAL::StackFramePtr task, const KID kid) noexcept {
  if (!kid || task == nullptr) return FALSE;
  return KernelTaskHelper::Add(task, kid);
}

/***********************************************************************************/
/// @brief Executes a new process from a function. Kernel code only.
/// @note This sets up a new stack, anything on the main function that calls the Kernel will not be
/// accessible.
/// @param main the start of the process.
/// @return if the process was started or not.
/***********************************************************************************/

ProcessID rtl_create_user_process(rtl_main_kind main, const Char* process_name) noexcept {
  if (!process_name || *process_name == 0) return kSchedInvalidPID;
  return UserProcessScheduler::The().Spawn(process_name, reinterpret_cast<VoidPtr>(main), nullptr);
}
}  // namespace Kernel
