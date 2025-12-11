/* ========================================

   Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

   ======================================== */

#include <LaunchKit/Foundation.h>
#include <libSystem/SystemKit/Err.h>
#include <libSystem/SystemKit/Syscall.h>

/// @note This called by _NeMain from its own runtime.
IMPORT_C SInt32 nelaunch_startup_fn(Void) {
  /// Start LaunchHelpers.fwrk services, and make the launcher manageable too (via mgmt.launch)
  UInt32* ret = static_cast<UInt32*>(libsys_syscall_arg_1(libsys_hash_64("__launch_register_launch_service")));

  if (ret) {
    switch (*ret) {
      case kErrorSuccess: {
        libsys_syscall_arg_1(libsys_hash_64("__launch_listen_as_root"));
        return *ret;
      }
      default:
        break;
    }
  }

  return kErrorExecutable;
}
