// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef HOSTKIT_FOUNDATION_H
#define HOSTKIT_FOUNDATION_H

#include <CoreFoundation/headers/Ref.h>
#include <SystemKit/System.h>

/// @author Amlal El Mahrouss
/// @brief The NeKernel Host Kit - host programs from it.

#define HOSTKIT_INFO(MSG) PrintOut(nullptr, "INFO: [LAUNCH] %s\n", MSG)
#define HOSTKIT_WARN(MSG) PrintOut(nullptr, "WARN: [LAUNCH] %s\n", MSG)

namespace HostKit {

using AnyRef    = CF::CFRef<VoidPtr>;
using StatusRef = CF::CFRef<SInt32>;

}  // namespace HostKit

#endif
