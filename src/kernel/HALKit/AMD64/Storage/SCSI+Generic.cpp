// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#include <modules/SCSI/SCSI.h>

using namespace Kernel;

///! @brief ATAPI SCSI packet.
const ATTRIBUTE(unused) scsi_packet_type_12 kCDRomPacketTemplate = {0x43, 0,  1,    0, 0, 0,
                                                                    0,    12, 0x40, 0, 0};
