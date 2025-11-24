/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.
  NeKernel is licensed under the Apache License 2.0 license.

======================================== */

#pragma once

#include <CompilerKit/CompilerKit.h>

namespace Kernel {
namespace Detail {
  inline constexpr auto kDebugCmdLen  = 256U;
  inline constexpr auto kDebugPort    = 51820;
  inline constexpr auto kDebugMagic   = "NE1.0.0;";
  inline constexpr auto kDebugVersion = 0x0100;
  inline constexpr auto kDebugDelim   = ';';
  inline constexpr auto kDebugEnd     = '\r';
  typedef UInt64        dk_socket_type;
}  // namespace Detail
}  // namespace Kernel