/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.
  NeKernel is licensed under the Apache License 2.0 license.

======================================== */

#ifndef __KERNELKIT_TRACESRV_H__
#define __KERNELKIT_TRACESRV_H__

#include <CompilerKit/CompilerKit.h>

namespace Kernel {
namespace Detail {
  inline constexpr auto kDebugCmdLen  = 256U;
  inline constexpr auto kDebugPort    = 51820;
  inline constexpr auto kDebugMagic   = "NE1.0.0;";
  inline constexpr auto kDebugVersion = 0x0100;
  inline constexpr auto kDebugDelim   = ';';
  inline constexpr auto kDebugEnd     = '\r';

  using dk_socket_type = UInt64;
}  // namespace Detail
}  // namespace Kernel

#endif  // !__KERNELKIT_TRACESRV_H__