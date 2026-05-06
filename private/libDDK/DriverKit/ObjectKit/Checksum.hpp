// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef DRIVERKIT_CHECKSUM_H
#define DRIVERKIT_CHECKSUM_H

#include <DriverKit/ObjectKit/Base.hpp>

namespace Kernel::DDK {

using Blob = char*;
using IndexableBlob = Blob;
using Indexable     = char;

/// @note This uses the FNV 64-bit variant.
/// @brief Performs a checksum computation for a memory region.
inline uintptr_t rtl_checksum(const IndexableBlob path) {
  if (!path || *path == 0) return 0;

  const uintptr_t kFNVSeed  = 0xcbf29ce484222325ULL;
  const uintptr_t kFNVPrime = 0x100000001b3ULL;

  uintptr_t hash = kFNVSeed;
  IndexableBlob path_ = path;

  while (*path_) {
    hash ^= (Indexable) (*path_);
    ++path_;

    hash *= kFNVPrime;
  }

  return hash;
}

}  // namespace Kernel::DDK

#endif
