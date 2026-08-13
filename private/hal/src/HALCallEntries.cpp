// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <hal/HAL/HAL.h>

EXTERN_C SInt32 hali_add_entry(HAL::hal_proc_type, UInt64, SInt64 hash);
EXTERN_C Void hali_remove_entry(HAL::hal_proc_type, UInt64, SInt64 hash);

struct HAL_DISPATCH_TABLE;
