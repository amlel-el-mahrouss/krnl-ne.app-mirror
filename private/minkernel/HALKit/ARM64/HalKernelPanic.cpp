// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

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

namespace Ne::Kernel {
/// @brief Dumping factory class.
class RecoveryFactory final {
 public:
  STATIC Void Recover();
};

/***********************************************************************************/
/// @brief Stops execution of the kernel.
/// @param id kernel stop ID.
/***********************************************************************************/
Void ke_stop(const Ne::Kernel::Int32& id, const Char* message) {
  (Void)(kout << "*** STOP ***\r");
  (Void)(kout << "Kernel_Panic_MSG: " << message << kendl);
  (Void)(kout << "Kernel_Panic_ID: " << hex_number(id) << kendl);

  RecoveryFactory::Recover();
}

Void RecoveryFactory::Recover() {
  Char in[2];
  in[0] = 0;

  /// @brief Begin packet recovery.
  TerminalDevice::The() << "\0x0F\0xFF # ";
  while (YES) {
    (Void)(TerminalDevice::The() >> in);
    in[1] = 0;

    if (in[0] >= 'a' && in[0] <= 'z') TerminalDevice::The() << in;
    if (in[0] >= '0' && in[0] <= '9') TerminalDevice::The() << in;
    /// @brief End packet recovery.
    if (in[0] == '\r') TerminalDevice::The() << "\r\xFF\0x0F# ";

    /// @brief Special characters for packet recovery.
    if (in[0] == '-') TerminalDevice::The() << "-";
    if (in[0] == ':') TerminalDevice::The() << ":";
    if (in[0] == '>') TerminalDevice::The() << ">";
    if (in[0] == ';') TerminalDevice::The() << ";";
    if (in[0] == '(') TerminalDevice::The() << "(";
    if (in[0] == ')') TerminalDevice::The() << ")";
    if (in[0] == ',') TerminalDevice::The() << ",";
    if (in[0] == '.') TerminalDevice::The() << ".";
  }
}

void ke_runtime_check(bool expr, const Char* file, const Char* line) {
  if (!expr) {
    (Void)(kout << "Kernel_Panic_FILE: " << file << kendl);
    (Void)(kout << "Kernel_Panic_LINE: " << line << kendl);

    ke_stop(RUNTIME_CHECK_FAILED, file);  // Runtime Check failed
  }
}
}  // namespace Ne::Kernel
