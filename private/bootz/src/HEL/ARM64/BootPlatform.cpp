// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <BootKit/BootKit.h>
#include <BootKit/Platform.h>
#include <BootKit/Protocol.h>

#ifdef __BOOTZ_STANDALONE__

using namespace Boot;

EXTERN_C void rt_halt() {
  while (Yes);
}

EXTERN_C void rt_cli() {}

EXTERN_C void rt_sti() {}

EXTERN_C void rt_cld() {}

EXTERN_C void rt_std() {}

#endif  // __BOOTZ_STANDALONE__
