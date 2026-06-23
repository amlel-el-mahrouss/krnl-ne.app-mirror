// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <StorageKit/SCSI.h>

namespace Ne::Kernel {
SCSIDeviceInterface::SCSIDeviceInterface(void (*out)(IDevice*, IMountpoint* outpacket),
                                         void (*in)(IDevice*, IMountpoint* inpacket),
                                         void (*cleanup)(void))
    : IDevice(out, in), fCleanup(cleanup) {}

SCSIDeviceInterface::~SCSIDeviceInterface() {
  if (fCleanup) fCleanup();
}

const Char* SCSIDeviceInterface::Name() const {
  return kDeviceMgrRootDirPath "sd{}{}";
}
}  // namespace Ne::Kernel
