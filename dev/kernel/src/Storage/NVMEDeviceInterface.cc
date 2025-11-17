/* -------------------------------------------

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

------------------------------------------- */

#include <StorageKit/NVME.h>

namespace Kernel {
NVMEDeviceInterface::NVMEDeviceInterface(
    void (*out)(DeviceInterface*, IMountpoint* outpacket),
    void (*in)(DeviceInterface*, IMountpoint* inpacket), void (*cleanup)(void))
    : DeviceInterface(out, in), fCleanup(cleanup) {}

NVMEDeviceInterface::~NVMEDeviceInterface() {
  if (fCleanup) fCleanup();
}

const Char* NVMEDeviceInterface::Name() const {
  return ("/devices/nvme{}");
}
}  // namespace Kernel
