// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel


#include <LaunchKit/Foundation.h>
#include <libSystem/SystemKit/Err.h>
#include <libSystem/SystemKit/Syscall.h>

/// @note This called by _NeMain from its own runtime.
IMPORT_C SInt32 launch_startup_fn(Void) {
  /// Start LaunchHelpers.fwrk services, and make the launcher manageable too (via mgmt.launch)
  UInt32* ret =
    static_cast<UInt32*>(libsys_syscall_arg_1(libsys_hash_64("__launch_register_service"))); // Register service based on program data.

  if (ret) {
    switch (*ret) {
      case kErrorSuccess: {
        ret =
          static_cast<UInt32*>(libsys_syscall_arg_1(libsys_hash_64("__launch_attach_service"))); // Attach this program as the service process.
        return *ret;
      }
      default:
        break;
    }
  }

  return kErrorExecutable;
}
