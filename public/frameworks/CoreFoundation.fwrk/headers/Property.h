/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <CoreFoundation.fwrk/headers/Foundation.h>
#include <CoreFoundation.fwrk/headers/Ref.h>
#include <libSystem/SystemKit/System.h>

#define kCFMaxPropLen (256U)

namespace CF {
class CFString;
class CFProperty;

template <typename Cls, SizeT N>
class CFArray;

/// ================================================================================
/// @brief handle to anything (number, ptr, string...)
/// ================================================================================
using CFPropertyId = UIntPtr;

/// ================================================================================
/// @brief User property class.
/// @example /prop/foo or /prop/bar
/// ================================================================================
class CFProperty final CF_OBJECT {
 public:
  CFProperty(CFRef<CFGUID> guid, CFString& name, CFPropertyId value);
  ~CFProperty() override = default;

 public:
  CFProperty& operator=(const CFProperty&) = default;
  CFProperty(const CFProperty&)            = default;

  Bool          StringEquals(CFString& name);
  CFPropertyId& GetValue();
  CFString&     GetKey();

 private:
  CFString*    fName{nullptr};
  CFPropertyId fValue{0UL};
  CFRef<CFGUID>  fGUID{};
};

template <SizeT N>
using CFPropertyArray = CFArray<CFProperty, N>;
}  // namespace CF

