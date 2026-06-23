// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef NEKIT_CRC32_H
#define NEKIT_CRC32_H

#include <NeKit/Config.h>

namespace Ne::Kernel {
UInt32 ke_calculate_crc32(const VoidPtr crc, Int32 len);
}  // namespace Ne::Kernel

#endif  // !NEKIT_CRC32_H
