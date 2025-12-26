/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

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
