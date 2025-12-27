// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef STORAGEKIT_SCSI_H
#define STORAGEKIT_SCSI_H

#include <KernelKit/DriveMgr.h>
#include <NeKit/OwnPtr.h>
#include <modules/SCSI/SCSI.h>

namespace Kernel {
class SCSIDeviceInterface final NE_DEVICE<IMountpoint*> {
 public:
  explicit SCSIDeviceInterface(Void (*out)(IDevice*, IMountpoint* out_packet),
                               Void (*in)(IDevice*, IMountpoint* in_packet), Void (*cleanup)(Void));

  ~SCSIDeviceInterface() override;

 public:
  NE_COPY_DEFAULT(SCSIDeviceInterface)

  const Char* Name() const override;

 private:
  Void (*fCleanup)(Void) = {nullptr};
};
}  // namespace Kernel

#endif
