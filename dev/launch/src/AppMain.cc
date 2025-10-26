/* -------------------------------------------

   Copyright (C) 2025, Amlal El Mahrouss, all rights reserved.

   ------------------------------------------- */

#include <LaunchKit/Foundation.h>
#include <libSystem/SystemKit/Err.h>
#include <libSystem/SystemKit/Syscall.h>

/// @note This called by _NeMain from its own runtime.
extern "C" SInt32 nelaunch_startup_fn(Void) {
  /// @todo Start LaunchServices.fwrk services, make the launcher manageable too (via mgmt.launch)

  NELAUNCH_INFO("Turning on launcher...");

  UInt32* ret = (UInt32*) libsys_syscall_arg_1(libsys_hash_64("__launch_register_pid0"));

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
