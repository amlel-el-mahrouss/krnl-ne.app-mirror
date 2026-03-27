// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef FW_HEADERS_FOUNDATION_H
#define FW_HEADERS_FOUNDATION_H

#include <libSystem/SystemKit/Err.h>
#include <libSystem/SystemKit/System.h>

#include <CoreFoundation.fwrk/headers/Object.h>
#include <CoreFoundation.fwrk/headers/String.h>

#define KT_TEST_VERSION_BCD (0x0001)
#define KT_TEST_VERSION "v0.0.1"

#define KT_TEST_SUCCESS (kErrorSuccess)
#define KT_TEST_FAILURE (kErrorSuccess + 1)

#ifndef __KT_TEST_MAIN
#define __KT_TEST_MAIN WinMain
#endif

#define KT_TEST_MAIN __KT_TEST_MAIN

#endif
