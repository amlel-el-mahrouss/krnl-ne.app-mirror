/* -------------------------------------------

   Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

   ------------------------------------------- */

#pragma once

#include <libSystem/SystemKit/System.h>
#include <CoreFoundation.fwrk/headers/Ref.h>

/// @author Amlal El Mahrouss
/// @brief NeKernel Launch Kit - launch programs from it.

#define NELAUNCH_INFO(MSG) PrintOut(nullptr, "INFO: [LAUNCH] %s\n", MSG)
#define NELAUNCH_WARN(MSG) PrintOut(nullptr, "WARN: [LAUNCH] %s\n", MSG)

namespace Launch {
using AnyRef    = CF::CFRef<VoidPtr>;
using StatusRef = CF::CFRef<SInt32>;
}  // namespace Launch
