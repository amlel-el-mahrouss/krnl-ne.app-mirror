// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

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