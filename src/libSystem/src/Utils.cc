/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#include <libSystem/SystemKit/Err.h>
#include <libSystem/SystemKit/Syscall.h>
#include <libSystem/SystemKit/System.h>
#include <libSystem/SystemKit/Verify.h>

using namespace LibSystem;

/// @note This uses the FNV 64-bit variant.
IMPORT_C UInt64 libsys_hash_64(const Char* path) {
  if (!path || *path == 0) return 0;

  const UInt64 kFNVSeed  = 0xcbf29ce484222325ULL;
  const UInt64 kFNVPrime = 0x100000001b3ULL;

  UInt64 hash = kFNVSeed;

  while (*path) {
    hash ^= (Char) (*path++);
    hash *= kFNVPrime;
  }

  return hash;
}
