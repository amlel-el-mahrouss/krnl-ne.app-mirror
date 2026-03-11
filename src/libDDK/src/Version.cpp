// SPDX-License-Identifier: Apache-2.0
// Copyright 2025-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (See accompanying
// file LICENSE or copy at http://www.apache.org/licenses/LICENSE-2.0)
// Official repository: https://github.com/ne-foss-org/ne_kernel

#include <DriverKit/DriverKit.h>

#ifndef kDDKVersionHighest
#define kDDKVersionHighest 1
#endif  // !kDDKVersionHighest

#ifndef kDDKVersionLowest
#define kDDKVersionLowest 1
#endif  // !kDDKVersionLowest

#ifndef kDDKVersion
#define kDDKVersion 1
#endif  // !kDDKVersion

uint32_t kApiVersionHighest = kDDKVersionHighest;
uint32_t kApiVersionLowest  = kDDKVersionLowest;
uint32_t kApiVersion        = kDDKVersion;
