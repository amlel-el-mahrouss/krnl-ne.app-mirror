// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel


#ifndef DRIVERKIT_STR_H
#define DRIVERKIT_STR_H

#include <DriverKit/DriverKit.h>

/// @brief DDK equivalent of POSIX's string.h
/// @file str.h

DDK_EXTERN size_t kstrlen(const char* in);
DDK_EXTERN int    kstrncpy(char* dst, const char* src, size_t len);

#endif
