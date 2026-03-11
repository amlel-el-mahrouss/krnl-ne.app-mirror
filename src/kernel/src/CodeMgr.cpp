// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel


#include <KernelKit/CodeMgr.h>
#include <KernelKit/ProcessScheduler.h>
#include <NeKit/Utils.h>

namespace Kernel {

/// @brief Executes a new process from a function. Kernel code only.
/// @note This sets up a new stack, anything on the main function that calls the Kernel will not be
/// accessible.
/// @param main the start of the process.
/// @param kid the Kernel ID of the new task.
/// @return The process started or not.
BOOL rtl_create_kernel_task(KernelTask& task, const KID& kid) {
  if (!kid) return FALSE;
  return KernelTaskHelper::Start(task, kid);
}

/***********************************************************************************/
/// @brief Executes a new process from a function. Kernel code only.
/// @note This sets up a new stack, anything on the main function that calls the Kernel will not be
/// accessible.
/// @param main the start of the process.
/// @return if the process was started or not.
/***********************************************************************************/

ProcessID rtl_create_user_process(rtl_start_kind main, const Char* process_name) {
  if (!process_name || *process_name == 0) return kSchedInvalidPID;
  return UserProcessScheduler::The().Spawn(process_name, reinterpret_cast<VoidPtr>(main), nullptr);
}

}  // namespace Kernel
