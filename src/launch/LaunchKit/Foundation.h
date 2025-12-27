// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef LAUNCHKIT_FOUNDATION_H
#define LAUNCHKIT_FOUNDATION_H

#include <CoreFoundation.fwrk/headers/Ref.h>
#include <libSystem/SystemKit/System.h>

/// @author Amlal El Mahrouss
/// @brief NeKernel Launch Kit - launch programs from it.

#define NELAUNCH_INFO(MSG) PrintOut(nullptr, "INFO: [LAUNCH] %s\n", MSG)
#define NELAUNCH_WARN(MSG) PrintOut(nullptr, "WARN: [LAUNCH] %s\n", MSG)

namespace LaunchKit {
using AnyRef    = CF::CFRef<VoidPtr>;
using StatusRef = CF::CFRef<SInt32>;
}  // namespace LaunchKit

#endif
