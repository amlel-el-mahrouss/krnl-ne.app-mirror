/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef KERNELKIT_PROCESSSCHEDULER_H
#define KERNELKIT_PROCESSSCHEDULER_H

#include <KernelKit/KernelTaskScheduler.h>
#include <KernelKit/UserProcessScheduler.h>

#ifdef __NEOSKRNL__
namespace Kernel {
inline UserProcessTeam kLowUserTeam;
inline UserProcessTeam kHighUserTeam;
inline UserProcessTeam kMidUserTeam;
inline UserProcessTeam kRTUserTeam;
}  // namespace Kernel
#endif

#endif