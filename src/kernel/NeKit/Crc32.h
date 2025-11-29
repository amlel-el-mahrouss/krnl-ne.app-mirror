/*
 *	========================================================
 *
 *  NeKernel
 *  Date Added: 13/02/2023
 * 	Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.
 *
 * 	========================================================
 */

#ifndef NEKIT_CRC32_H
#define NEKIT_CRC32_H

#include <NeKit/Defines.h>

namespace Kernel {
UInt32 ke_calculate_crc32(const VoidPtr crc, Int32 len);
}  // namespace Kernel

#endif  // !NEKIT_CRC32_H
