/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#include <StorageKit/SCSI.h>

namespace Kernel {
SCSIDeviceInterface::SCSIDeviceInterface(void (*out)(DeviceInterface*, IMountpoint* outpacket),
                                         void (*in)(DeviceInterface*, IMountpoint* inpacket),
                                         void (*cleanup)(void))
    : DeviceInterface(out, in), fCleanup(cleanup) {}

SCSIDeviceInterface::~SCSIDeviceInterface() {
  if (fCleanup) fCleanup();
}

const Char* SCSIDeviceInterface::Name() const {
  return ("/devices/sda{}");
}
}  // namespace Kernel
