/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef CFKIT_PROPS_H
#define CFKIT_PROPS_H

#include <CFKit/GUIDWrapper.h>
#include <NeKit/Array.h>
#include <NeKit/Config.h>
#include <NeKit/Function.h>
#include <NeKit/KString.h>

#define kMaxPropLen (256U)

namespace Kernel::CF {
/// @brief handle to anything (number, ptr, string...)
using PropertyId = UIntPtr;

/// @brief Kernel property class.
/// @note /prop/smp_max or /prop/kern_ver are properties.
class Property {
 public:
  Property();
  virtual ~Property();

 public:
  Property& operator=(const Property&) = default;
  Property(const Property&)            = default;

  BOOL            StringEquals(KBasicString<>& name);
  PropertyId&     GetValue();
  KBasicString<>& GetKey();

 private:
  KString        fName{kMaxPropLen};
  PropertyId     fValue{0UL};
  Ref<XRN::GUID> fGUID{};
};

template <SizeT N>
using PropertyArray = Array<Property, N>;
}  // namespace Kernel::CF

namespace Kernel {
using namespace Kernel::CF;
}

#endif  // !CFKIT_PROPS_H
