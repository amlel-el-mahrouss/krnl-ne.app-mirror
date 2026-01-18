// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef HEADERS_PROPERTY_H
#define HEADERS_PROPERTY_H

#include <CoreFoundation.fwrk/headers/Foundation.h>
#include <CoreFoundation.fwrk/headers/Ref.h>
#include <libSystem/SystemKit/System.h>

#define kCFMaxPropLen (256U)

namespace CF {
class CFString;
class CFProperty;

template <class Cls, SizeT N>
class CFArray;

/// ================================================================================
/// @brief handle to anything (number, ptr, string...)
/// ================================================================================
using CFPropertyId = UIntPtr;

/// ================================================================================
/// @brief User property class.
/// @note /prop/foo or /prop/bar are properties.
/// ================================================================================
class CFProperty final CF_OBJECT {
 public:
  CFProperty(CFRef<CFGuid> guid, CFString& name, CFPropertyId value);
  ~CFProperty() override = default;

 public:
  CFProperty& operator=(const CFProperty&) = default;
  CFProperty(const CFProperty&)            = default;

  Bool          StringEquals(CFString& name);
  CFPropertyId& GetValue();
  CFString&     GetKey();

 private:
  CFString*     fName{nullptr};
  CFPropertyId  fValue{0UL};
  CFRef<CFGuid> fGUID{};
};

template <SizeT N>
using CFPropertyArray = CFArray<CFProperty, N>;
}  // namespace CF

#endif
