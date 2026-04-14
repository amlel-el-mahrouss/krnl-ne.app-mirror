// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <ArchKit/ArchKit.h>
#include <FirmwareKit/Handover.h>
#include <KernelKit/DebugOutput.h>
#include <KernelKit/FileMgr.h>
#include <KernelKit/Timer.h>
#include <NeKit/KString.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/Utils.h>
#include <modules/CoreGfx/CoreGfx.h>
#include <modules/CoreGfx/TextGfx.h>

/* Each error code is attributed with an ID, which will prompt a string onto the
 * screen. Wait for debugger... */

namespace Kernel {
/// @brief Dumping factory class.
class RecoveryFactory final {
 public:
  STATIC Void Recover();
};

/***********************************************************************************/
/// @brief Stops execution of the kernel.
/// @param id kernel stop ID.
/***********************************************************************************/
Void ke_stop(const Kernel::Int32& id, const Char* message) {
  (Void)(kout << "*** STOP ***\r");
  (Void)(kout << "Kernel_Panic_MSG: " << message << kendl);
  (Void)(kout << "Kernel_Panic_ID: " << hex_number(id) << kendl);

  RecoveryFactory::Recover();
}

Void RecoveryFactory::Recover() {
  while (YES) {
    HAL::rt_halt();
  }
}

void ke_runtime_check(bool expr, const Char* file, const Char* line) {
  if (!expr) {
    (Void)(kout << "Kernel_Panic_FILE: " << file << kendl);
    (Void)(kout << "Kernel_Panic_LINE: " << line << kendl);

    ke_stop(RUNTIME_CHECK_FAILED, file);  // Runtime Check failed
  }
}
}  // namespace Kernel
