// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#include <DriverKit/ddk.h>

void* operator new(size_t sz) {
  if (!sz) ++sz;

  return ::kalloc(sz);
}

void operator delete(void* ptr) {
  ::kfree(ptr);
}

void* operator new[](size_t sz) {
  if (!sz) ++sz;

  return ::kalloc(sz);
}

void operator delete[](void* ptr) {
  ::kfree(ptr);
}
