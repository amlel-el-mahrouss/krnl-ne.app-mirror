/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#include <KernelKit/Timer.h>

///! BUGS: 0
///! @file Timer.cc
///! @brief Software Timer implementation
///! @author Amlal El Mahrouss (amlal@nekernel.org)

namespace Kernel {
/// @brief Unimplemented as it is an interface.
BOOL ITimer::Wait() {
  return NO;
}
}  // namespace Kernel