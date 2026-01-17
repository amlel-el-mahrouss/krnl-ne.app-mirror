// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#include <StorageKit/AHCI.h>

using namespace Kernel;

/// @brief Class constructor
/// @param Out Drive output
/// @param In  Drive input
/// @param Cleanup Drive cleanup.
AHCIDeviceInterface::AHCIDeviceInterface(void (*out)(IDevice* self, IMountpoint* outpacket),
                                         void (*in)(IDevice* self, IMountpoint* inpacket))
    : IDevice(out, in) {}

/// @brief Class desctructor
AHCIDeviceInterface::~AHCIDeviceInterface() = default;

/// @brief Returns the name of the device interface.
/// @return it's name as a string.
const Char* AHCIDeviceInterface::Name() const {
  return kDeviceMgrRootDirPath "sd{}{}";
}

/// @brief Output operator.
/// @param mnt the disk mountpoint.
/// @return the class itself after operation.
AHCIDeviceInterface& AHCIDeviceInterface::operator<<(IMountpoint* mnt) {
  if (!mnt) return *this;

  for (SizeT driveCount = 0; driveCount < kDriveMaxCount; ++driveCount) {
    auto interface = mnt->GetAddressOf(driveCount);

    if ((interface) &&
        rt_string_cmp((interface)->fProtocol(), "AHCI", rt_string_len("AHCI")) == 0) {
      continue;
    } else if ((interface) &&
               rt_string_cmp((interface)->fProtocol(), "AHCI", rt_string_len("AHCI")) != 0) {
      return *this;
    }
  }

  return (AHCIDeviceInterface&) IDevice<IMountpoint*>::operator<<(mnt);
}

/// @brief Input operator.
/// @param mnt the disk mountpoint.
/// @return the class itself after operation.
AHCIDeviceInterface& AHCIDeviceInterface::operator>>(IMountpoint* mnt) {
  if (!mnt) return *this;

  for (SizeT driveCount = 0; driveCount < kDriveMaxCount; ++driveCount) {
    auto interface = mnt->GetAddressOf(driveCount);

    // really check if it's AHCI.
    if ((interface) &&
        rt_string_cmp((interface)->fProtocol(), "AHCI", rt_string_len("AHCI")) == 0) {
      continue;
    } else if ((interface) &&
               rt_string_cmp((interface)->fProtocol(), "AHCI", rt_string_len("AHCI")) != 0) {
      return *this;
    }
  }

  return (AHCIDeviceInterface&) IDevice<IMountpoint*>::operator>>(mnt);
}

const UInt16& AHCIDeviceInterface::GetPortsImplemented() {
  return this->fPortsImplemented;
}

Void AHCIDeviceInterface::SetPortsImplemented(const UInt16& pi) {
  MUST_PASS(pi > 0);
  this->fPortsImplemented = pi;
}

const UInt32& AHCIDeviceInterface::GetIndex() {
  return this->fDriveIndex;
}

Void AHCIDeviceInterface::SetIndex(const UInt32& drv) {
  MUST_PASS(IMountpoint::kDriveIndexInvalid < drv);
  this->fDriveIndex = drv;
}