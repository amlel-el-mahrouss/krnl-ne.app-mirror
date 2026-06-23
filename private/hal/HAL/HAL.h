// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/ne-kernel

// SPDX-License-Identifier: Proprietary
// Copyright 2026, Ne.app, all rights reserved.

#ifndef HAL_HAL_H
#define HAL_HAL_H

#include <ArchKit/ArchKit.h>

#ifndef HAL_UNUSED
#define HAL_UNUSED(X) (Void) X
#endif

namespace Ne::Kernel::HAL {

struct HAL_CALL_ENTRY;

using hal_proc_type = Ne::Kernel::Void (*)(Ne::Kernel::VoidPtr);

enum : SInt64 { kAuthLevelInvalid, kAuthLevelHigh = 100, kAuthLevelMid, kAuthLevelLow };

/// @brief Entry structure of the HAL dispatch table.
/// @authors Amlal El Mahrouss (amlal@nekernel.org, amlal@ne-app.eu)

struct HAL_CALL_ENTRY final {
  UInt64        fHash;
  Bool          fActive;
  hal_proc_type fProc;
  SInt64        fAuthLevel;

  BOOL     IsActive() { return fActive; }
  explicit operator bool() { return fActive; }
};

inline Array<HAL_CALL_ENTRY, kMaxDispatchCallCount> kRegisteredSystemCalls;

}  // namespace Ne::Kernel::HAL

#endif  // HAL_HAL_H