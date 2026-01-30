// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef DRIVERKIT_CHECKSUM_H
#define DRIVERKIT_CHECKSUM_H

#include <DriverKit/c++/driver_base.h>

namespace Kernel::DDK {

using IndexableBlob = Char*;

/// @note This uses the FNV 64-bit variant.
/// @brief Performs a checksum computation for a memory region.
inline UInt64 rtl_checksum(const IndexableBlob path) {
  if (!path || *path == 0) return 0;

  const UInt64 kFNVSeed  = 0xcbf29ce484222325ULL;
  const UInt64 kFNVPrime = 0x100000001b3ULL;

  UInt64 hash = kFNVSeed;

  IndexableBlob path_ = path;

  while (*path_) {
    hash ^= (Char) (*path_++);
    hash *= kFNVPrime;
  }

  return hash;
}

using Blob = VoidPtr;

}  // namespace Kernel::DDK

#endif
