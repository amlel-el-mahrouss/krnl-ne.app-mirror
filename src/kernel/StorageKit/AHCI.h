// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef STORAGEKIT_AHCI_H
#define STORAGEKIT_AHCI_H

#include <KernelKit/DeviceMgr.h>
#include <KernelKit/DriveMgr.h>
#include <NeKit/OwnPtr.h>

namespace Kernel {
/// @brief AHCIDeviceInterface class
/// @details This class is used to send and receive data from the AHCI device.
/// @note The class is derived from the IDevice class.
class AHCIDeviceInterface NE_DEVICE<IMountpoint*> {
 public:
  explicit AHCIDeviceInterface(void (*out)(IDevice* self, IMountpoint* out),
                               void (*in)(IDevice* self, IMountpoint* in));

  virtual ~AHCIDeviceInterface() override;

 public:
  AHCIDeviceInterface& operator=(const AHCIDeviceInterface&) = default;
  AHCIDeviceInterface(const AHCIDeviceInterface&)            = default;

  const Char* Name() const override;

  const UInt16& GetPortsImplemented();
  Void          SetPortsImplemented(const UInt16& pi);

  const UInt32& GetIndex();
  Void          SetIndex(const UInt32& drv);

 public:
  AHCIDeviceInterface& operator<<(IMountpoint* Data) override;
  AHCIDeviceInterface& operator>>(IMountpoint* Data) override;

 private:
  UInt16 fPortsImplemented{0U};
  UInt32 fDriveIndex{0U};
};

UInt16                       sk_init_ahci_device(BOOL atapi);
ErrorOr<AHCIDeviceInterface> sk_acquire_ahci_device(UInt32 drv_index);
}  // namespace Kernel

#endif
