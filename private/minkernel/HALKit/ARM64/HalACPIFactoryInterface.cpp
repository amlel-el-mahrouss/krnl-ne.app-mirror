// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <ArchKit/ArchKit.h>
#include <KernelKit/HeapMgr.h>
#include <NeKit/KString.h>
#include <modules/ACPI/ACPIFactoryInterface.h>
#include <modules/APM/APM.h>

namespace Kernel {
ACPIFactoryInterface::ACPIFactoryInterface(VoidPtr rsp_ptr) : fRsdp(rsp_ptr), fEntries(0) {}

BOOL ACPIFactoryInterface::Shutdown() {
  apm_send_io_command(kAPMPowerCommandShutdown);
  return NO;
}

/// @brief Reboot machine in either ACPI or by triple faulting.
/// @return nothing it's a reboot.
Void ACPIFactoryInterface::Reboot() {
  apm_send_io_command(kAPMPowerCommandReboot);
}
}  // namespace Kernel
