// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef DRIVERKIT_IO_H
#define DRIVERKIT_IO_H

#include <DriverKit/String.h>

/// @brief print character into UART.
DDK_EXTERN void kputc(const char ch);

/// @brief print string to UART.
/// @param message string to transmit to UART.
DDK_EXTERN void kprint(const char* message);

#endif
