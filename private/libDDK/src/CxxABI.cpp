// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (See accompanying
// file LICENSE or copy at http://www.apache.org/licenses/LICENSE-2.0)
// Official repository: https://github.com/ne-foss/ne-kernel

#include <DriverKit/DriverKit.h>

void* operator new(size_t sz) {
  if (!sz) ++sz;

  return ::kalloc(sz);
}

void operator delete(void* ptr) noexcept {
  ::kfree(ptr);
}

void* operator new[](size_t sz) {
  if (!sz) ++sz;

  return ::kalloc(sz);
}

void operator delete[](void* ptr) noexcept {
  ::kfree(ptr);
}
