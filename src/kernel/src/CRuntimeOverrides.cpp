// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#include <NeKit/Utils.h>

using namespace Kernel;

/// =========================================================== ///
/// @brief C Standard Library overrides.                     ///
/// =========================================================== ///

EXTERN_C void* memset(void* dst, int c, long long unsigned int len) {
  return Kernel::rt_set_memory_safe(dst, c, static_cast<Size>(len), static_cast<Size>(len));
}

EXTERN_C void* memcpy(void* dst, const void* src, long long unsigned int len) {
  Kernel::rt_copy_memory_safe(const_cast<void*>(src), dst, static_cast<Size>(len),
                              static_cast<Size>(len));
  return dst;
}

EXTERN_C Int32 strcmp(const char* a, const char* b) {
  return Kernel::rt_string_cmp(a, b, rt_string_len(a));
}
