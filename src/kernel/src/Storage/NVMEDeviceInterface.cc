/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#include <StorageKit/NVME.h>

namespace Kernel {
NVMEDeviceInterface::NVMEDeviceInterface(void (*out)(IDevice*, IMountpoint* outpacket),
                                         void (*in)(IDevice*, IMountpoint* inpacket),
                                         void (*cleanup)(void))
    : IDevice(out, in), fCleanup(cleanup) {}

NVMEDeviceInterface::~NVMEDeviceInterface() {
  if (fCleanup) fCleanup();
}

const Char* NVMEDeviceInterface::Name() const {
  return ("/devices/nvm{}");
}

OwnPtr<IMountpoint*> NVMEDeviceInterface::operator()(UInt32 dma_low, UInt32 dma_high,
                                                     SizeT dma_sz) {
  NE_UNUSED(dma_low);
  NE_UNUSED(dma_high);
  NE_UNUSED(dma_sz);

  return {};
}
}  // namespace Kernel
