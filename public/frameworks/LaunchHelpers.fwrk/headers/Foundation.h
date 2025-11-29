/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <CoreFoundation.fwrk/headers/Foundation.h>
#include <CoreFoundation.fwrk/headers/String.h>

namespace LaunchHelpers {
struct LHLaunchInfo;

/// @brief Launch information structure.
/// @note This structure is read-only. Modyfing its members wo't have any effect.
struct LHLaunchInfo final {
  CF::CFString    fExecutablePath;
  CF::CFString    fWorkingDirectory;
  CF::CFString    fArguments;
  CF::CFString    fEnvironment;
  CF::CFInteger64 fUID{0};
  CF::CFInteger64 fGID{0};
};

/// @brief Get launch information.
/// @return the launch information structure.
LHLaunchInfo* LHGetLaunchInfo(Void);
}  // namespace LaunchHelpers