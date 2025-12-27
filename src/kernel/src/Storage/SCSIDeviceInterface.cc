// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#include <StorageKit/SCSI.h>

namespace Kernel {
SCSIDeviceInterface::SCSIDeviceInterface(void (*out)(IDevice*, IMountpoint* outpacket),
                                         void (*in)(IDevice*, IMountpoint* inpacket),
                                         void (*cleanup)(void))
    : IDevice(out, in), fCleanup(cleanup) {}

SCSIDeviceInterface::~SCSIDeviceInterface() {
  if (fCleanup) fCleanup();
}

const Char* SCSIDeviceInterface::Name() const {
  return ("/devices/sda{}");
}
}  // namespace Kernel
