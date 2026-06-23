// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef CFKIT_GUIDWRAPPER_H
#define CFKIT_GUIDWRAPPER_H

#include <NeKit/Config.h>
#include <NeKit/Ref.h>
#include <NeKit/Stream.h>

/* GUID for C++ classes. */

#define kXRNNil "@{........-....-M...-N...-............}"

/// @brief eXtended Resource Namespace
namespace Ne::Kernel::CF::XRN {
union GUIDSequence final {
  alignas(8) UShort fU8[16];
  alignas(8) UShort fU16[8];
  alignas(8) UInt fU32[4];
  alignas(8) ULong fU64[2];

  struct GUID {
    alignas(8) UInt fMs1;
    UShort fMs2;
    UShort fMs3;
    UChar  fMs4[8];
  } fUuid;
};

class GUID final {
 public:
  explicit GUID() = default;
  ~GUID()         = default;

 public:
  NE_COPY_DEFAULT(GUID)

 public:
  GUIDSequence& operator->() { return fUUID; }
  GUIDSequence& Leak() { return fUUID; }

 private:
  GUIDSequence fUUID;
};
}  // namespace Ne::Kernel::CF::XRN

#endif
