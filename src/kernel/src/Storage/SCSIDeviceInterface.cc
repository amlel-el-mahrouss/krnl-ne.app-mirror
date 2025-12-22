/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

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
