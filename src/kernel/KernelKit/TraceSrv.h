// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef __KERNELKIT_TRACESRV_H__
#define __KERNELKIT_TRACESRV_H__

#include <CompilerKit/CompilerKit.h>

namespace Kernel {

namespace TraceSrv {

  inline constexpr auto kDebugCmdLen = 256U;
  inline constexpr auto kDebugPort   = 51820;
  /// \brief Debug Magic Value
  inline constexpr auto kDebugMagic   = "NE1.0.0;";
  inline constexpr auto kDebugVersion = 0x0100;
  inline constexpr auto kDebugDelim   = ';';
  inline constexpr auto kDebugEnd     = '\r';

}  // namespace TraceSrv

}  // namespace Kernel

#endif  // !__KERNELKIT_TRACESRV_H__
