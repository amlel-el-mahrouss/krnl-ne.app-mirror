// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef SCSI_SCSI_H
#define SCSI_SCSI_H

#include <NeKit/Config.h>

/// @file SCSI.h
/// @brief Small Computer System Interface device.

namespace Ne::Kernel {
template <Int32 PacketBitLen>
using scsi_packet_type = Ne::Kernel::UInt16[PacketBitLen];

using scsi_packet_type_12 = scsi_packet_type<12>;

extern const scsi_packet_type_12 kCDRomPacketTemplate;
}  // namespace Ne::Kernel

#endif
