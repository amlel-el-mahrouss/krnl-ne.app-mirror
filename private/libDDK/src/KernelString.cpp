// SPDX-License-Identifier: Apache-2.0
// Copyright 2025-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (See accompanying
// file LICENSE or copy at http://www.apache.org/licenses/LICENSE-2.0)
// Official repository: https://github.com/ne-foss/ne-kernel

#include <DriverKit/String.h>

DDK_EXTERN size_t kstrlen(const char* in) {
  if (in == nil) return 0;

  if (*in == 0) return 0;

  size_t index = 0;

  while (in[index] != 0) {
    ++index;
  }

  return index;
}

DDK_EXTERN int kstrncpy(char* dst, const char* src, size_t len) {
  if (nil == dst || nil == src) return 0;

  size_t index = 0;

  while (index != len) {
    dst[index] = src[index];
    ++index;
  }

  return index;
}
