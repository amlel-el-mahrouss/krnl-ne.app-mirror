// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef SCSI_SCSI_H
#define SCSI_SCSI_H

#include <NeKit/Config.h>

/// @file SCSI.h
/// @brief Small Computer System Interface device.

namespace Kernel {
template <Int32 PacketBitLen>
using scsi_packet_type = Kernel::UInt16[PacketBitLen];

using scsi_packet_type_12 = scsi_packet_type<12>;

extern const scsi_packet_type_12 kCDRomPacketTemplate;
}  // namespace Kernel

#endif
