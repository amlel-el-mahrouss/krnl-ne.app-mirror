/*
 *	========================================================
 *
 *  NeKernel
 *  Date Added: 13/02/2023
 * 	Copyright (C) 2024-2025, Amlal El Mahrouss, all rights reserved.
 *
 * 	========================================================
 */

#ifndef NEKIT_CRC32_H
#define NEKIT_CRC32_H

#include <NeKit/Defines.h>

namespace Kernel {
UInt32 ke_calculate_crc32(const VoidPtr crc, Int32 len) noexcept;
}  // namespace Kernel

#endif  // !NEKIT_CRC32_H
