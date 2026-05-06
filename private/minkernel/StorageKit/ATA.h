// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef STORAGEKIT_ATA_H
#define STORAGEKIT_ATA_H

#include <KernelKit/DeviceMgr.h>
#include <KernelKit/DriveMgr.h>
#include <NeKit/OwnPtr.h>
#include <NeKit/Utils.h>

namespace Kernel {
/// @brief ATA device interface class.
class ATADeviceInterface final NE_DEVICE<IMountpoint*> {
 public:
  ATADeviceInterface(void (*Out)(IDevice*, IMountpoint* outpacket),
                     void (*In)(IDevice*, IMountpoint* inpacket));

  virtual ~ATADeviceInterface();

 public:
  ATADeviceInterface& operator<<(IMountpoint* Data) override;
  ATADeviceInterface& operator>>(IMountpoint* Data) override;

 public:
  ATADeviceInterface& operator=(const ATADeviceInterface&) = default;
  ATADeviceInterface(const ATADeviceInterface&)            = default;

  const Char* Name() const override;

  const UInt16& GetIO();
  Void          SetIO(const UInt16& io);

  const UInt16& GetMaster();
  Void          SetMaster(const UInt16& master);

  const UInt32& GetIndex();
  Void          SetIndex(const UInt32& drv);

 private:
  UInt32 fDriveIndex{0U};
  UInt16 fIO, fMaster{0U};
};

/// @brief Initialize an PIO device (StorageKit function)
/// @param is_master is the current PIO master?
/// @return [io:master] for PIO device.
BOOL sk_init_ata_device(BOOL is_master, UInt16& io, UInt8& master);

/// @brief Acquires a new PIO device with drv_index in mind.
/// @param drv_index The drive index to assign.
/// @return A wrapped device interface if successful, or error code.
ErrorOr<ATADeviceInterface> sk_acquire_ata_device(Int32 drv_index);
}  // namespace Kernel

#endif
