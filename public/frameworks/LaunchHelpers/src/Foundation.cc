// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <LaunchHelpers/headers/Foundation.h>
#include <libSystem/SystemKit/Syscall.h>

/// @brief Get launch information.
/// @return the launch information structure.
CF::CFRef<LaunchHelpers::LHLaunchInfo> LaunchHelpers::LHGetLaunchInfo(Void) {
  return static_cast<LaunchHelpers::LHLaunchInfo*>(
      libsys_syscall_arg_1(libsys_hash_64("__LHGetLaunchInfoRef")));
}