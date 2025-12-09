/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#include <LaunchHelpers.fwrk/headers/Foundation.h>
#include <libSystem/SystemKit/Syscall.h>

/// @brief Get launch information.
/// @return the launch information structure.
LaunchHelpers::LHLaunchInfo* LaunchHelpers::LHGetLaunchInfo(Void) {
  return static_cast<LaunchHelpers::LHLaunchInfo*>(
      libsys_syscall_arg_1(libsys_hash_64("__LHGetLaunchInfo")));
}