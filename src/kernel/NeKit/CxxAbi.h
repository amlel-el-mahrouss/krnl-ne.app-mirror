// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef NEKIT_CXXABI_H
#define NEKIT_CXXABI_H

#include <NeKit/Config.h>

#ifndef __NECTI__

#define kAtExitMacDestructors (128)

struct atexit_func_entry_t {
  void (*destructor_func)();
  void* obj_ptr;
  void* dso_handle;
};

typedef Kernel::UInt32 uarch_t;

namespace cxxabiv1 {
typedef Kernel::SizeT* __guard;
}

#endif  // !__NECTI__

#endif
