// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef STORAGEKIT_NVME_H
#define STORAGEKIT_NVME_H

#include <KernelKit/DeviceMgr.h>
#include <KernelKit/DriveMgr.h>

namespace Kernel {
class NVMEDeviceInterface final NE_DEVICE<IMountpoint*> {
 public:
  NVMEDeviceInterface(Void (*out)(IDevice*, IMountpoint* out_packet),
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
