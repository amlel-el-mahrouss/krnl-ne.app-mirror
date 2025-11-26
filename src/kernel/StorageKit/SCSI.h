/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <modules/SCSI/SCSI.h>
#include <KernelKit/DriveMgr.h>
#include <NeKit/OwnPtr.h>

namespace Kernel {
class SCSIDeviceInterface final NE_DEVICE<IMountpoint*> {
 public:
  explicit SCSIDeviceInterface(Void (*out)(DeviceInterface*, IMountpoint* out_packet),
                               Void (*in)(DeviceInterface*, IMountpoint* in_packet),
                               Void (*cleanup)(Void));

  ~SCSIDeviceInterface() override;

 public:
  NE_COPY_DEFAULT(SCSIDeviceInterface)

  const Char* Name() const override;

 private:
  Void (*fCleanup)(Void) = {nullptr};
};
}  // namespace Kernel
