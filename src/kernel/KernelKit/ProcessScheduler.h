/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <KernelKit/KernelTaskScheduler.h>
#include <KernelKit/UserProcessScheduler.h>

#ifdef __NEOSKRNL__
namespace Kernel {
inline UserProcessTeam kLowUserTeam;
inline UserProcessTeam kHighUserTeam;
inline UserProcessTeam kMidUserTeam;
}  // namespace Kernel
#endif