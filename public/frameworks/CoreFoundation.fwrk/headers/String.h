
/* -------------------------------------------

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

------------------------------------------- */

#pragma once

#include <CoreFoundation.fwrk/headers/Object.h>

namespace CF {
class CFString;

class CFString final CF_OBJECT {
 public:
  CFString()  = default;
  ~CFString() = default;

  CFString(const CFString&) = delete;
  CFString& operator=(const CFString&) = delete;
};
}  // namespace CF