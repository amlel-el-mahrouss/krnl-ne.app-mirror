/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <CoreFoundation.fwrk/headers/Foundation.h>
#include <CoreFoundation.fwrk/headers/String.h>

namespace LaunchHelpers {
struct LHLaunchInfo;

inline constexpr auto kMaxPath = 4096;
inline constexpr auto kMaxArgs = 256;

/// @brief Launch information structure.
/// @note This structure is read-only. Modyfing its members wo't have any effect.
struct LHLaunchInfo final {
  CF::CFString    fExecutablePath{kMaxPath};
  CF::CFString    fWorkingDirectory{kMaxPath};
  CF::CFRef<CF::CFString>    fArguments[kMaxArgs];
  CF::CFString    fEnvironment{kMaxPath};
  CF::CFInteger64 fUID{0};
  CF::CFInteger64 fGID{0};

  LHLaunchInfo() = default;
  ~LHLaunchInfo() = default;

  LIBSYS_COPY_DELETE(LHLaunchInfo)

  explicit operator bool() { return fUID && fGID; }

  CF::CFRef<CF::CFString>* begin() { return fArguments; }
  CF::CFRef<CF::CFString>* end() { return fArguments + kMaxArgs; }

  SizeT size() { return kMaxArgs; }
};

using LHLaunchInfoPtr = LHLaunchInfo*;

/// @brief Get launch information.
/// @return the launch information structure.
CF::CFRef<LHLaunchInfo> LHGetLaunchInfo(Void);
}  // namespace LaunchHelpers