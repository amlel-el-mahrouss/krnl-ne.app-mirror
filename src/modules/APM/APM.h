// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel
#ifndef APM_APM_H
#define APM_APM_H

#include <NeKit/Config.h>

namespace Kernel {
typedef Int32 APMPowerCmd;

enum {
  kAPMPowerCommandInvalid  = 0x00,
  kAPMPowerCommandStop     = 0x01,
  kAPMPowerCommandStart    = 0x02,
  kAPMPowerCommandSleep    = 0x04,
  kAPMPowerCommandWakeup   = 0x06,
  kAPMPowerCommandShutdown = 0x07,
  kAPMPowerCommandReboot   = 0x08,
};

/// @brief Send a APM command into it's controller. (Shutdown, Reboot, Sleep...)
/// @param base_dma the IO base port.
/// @param cmd the command.
/// @return status code of the APM command.
EXTERN_C Int32 apm_send_io_command(UInt16 cmd);
}  // namespace Kernel

#endif
