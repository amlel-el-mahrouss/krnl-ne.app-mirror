/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef STORAGEKIT_NVME_H
#define STORAGEKIT_NVME_H

#include <KernelKit/DeviceMgr.h>
#include <KernelKit/DriveMgr.h>

namespace Kernel {
class NVMEDeviceInterface final NE_DEVICE<IMountpoint*> {
 public:
  explicit NVMEDeviceInterface(Void (*out)(IDevice*, IMountpoint* out_packet),
                               Void (*in)(IDevice*, IMountpoint* in_packet), Void (*cleanup)(Void));

  ~NVMEDeviceInterface() override;

 public:
  NE_COPY_DEFAULT(NVMEDeviceInterface)

  const Char* Name() const override;

 public:
  OwnPtr<IMountpoint*> operator()(UInt32 dma_low, UInt32 dma_high, SizeT dma_sz);

 private:
  Void (*fCleanup)(Void) = {nullptr};
};
}  // namespace Kernel

#endif
