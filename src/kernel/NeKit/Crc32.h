// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef NEKIT_CRC32_H
#define NEKIT_CRC32_H

#include <NeKit/Config.h>

namespace Kernel {
UInt32 ke_calculate_crc32(const VoidPtr crc, Int32 len);
}  // namespace Kernel

#endif  // !NEKIT_CRC32_H
