// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#include <StorageKit/ATA.h>

using namespace Kernel;

/// @brief Class constructor
/// @param Out Drive output
/// @param In  Drive input
/// @param Cleanup Drive cleanup.
ATADeviceInterface::ATADeviceInterface(void (*Out)(IDevice*, IMountpoint* outpacket),
                                       void (*In)(IDevice*, IMountpoint* inpacket))
    : IDevice(Out, In) {}

/// @brief Class desctructor
ATADeviceInterface::~ATADeviceInterface() = default;

/// @brief Returns the name of the device interface.
/// @return it's name as a string.
const Char* ATADeviceInterface::Name() const {
  return "/devices/hda{}";
}

/// @brief Output operator.
/// @param Data the disk mountpoint.
/// @return the class itself after operation.
ATADeviceInterface& ATADeviceInterface::operator<<(IMountpoint* Data) {
  if (!Data) return *this;

  for (SizeT driveCount = 0; driveCount < kDriveMaxCount; ++driveCount) {
    auto interface = Data->GetAddressOf(driveCount);

    if ((interface) &&
        rt_string_cmp((interface)->fProtocol(), "ATA-", rt_string_len("ATA-")) == 0) {
      continue;
    } else if ((interface) &&
               rt_string_cmp((interface)->fProtocol(), "ATA-", rt_string_len("ATA-")) != 0) {
      return *this;
    }
  }

  return (ATADeviceInterface&) IDevice<IMountpoint*>::operator<<(Data);
}

/// @brief Input operator.
/// @param Data the disk mountpoint.
/// @return the class itself after operation.
ATADeviceInterface& ATADeviceInterface::operator>>(IMountpoint* Data) {
  if (!Data) return *this;

  for (SizeT driveCount = 0; driveCount < kDriveMaxCount; ++driveCount) {
    auto interface = Data->GetAddressOf(driveCount);

    // really check if it's ATA.
    if ((interface) &&
        rt_string_cmp((interface)->fProtocol(), "ATA-", rt_string_len("ATA-")) == 0) {
      continue;
    } else if ((interface) &&
               rt_string_cmp((interface)->fProtocol(), "ATA-", rt_string_len("ATA-")) != 0) {
      return *this;
    }
  }

  return (ATADeviceInterface&) IDevice<IMountpoint*>::operator>>(Data);
}

const UInt32& ATADeviceInterface::GetIndex() {
  return this->fDriveIndex;
}

Void ATADeviceInterface::SetIndex(const UInt32& drv) {
  MUST_PASS(IMountpoint::kDriveIndexInvalid < drv);
  this->fDriveIndex = drv;
}

const UInt16& ATADeviceInterface::GetIO() {
  return this->fIO;
}

Void ATADeviceInterface::SetIO(const UInt16& drv) {
  MUST_PASS(0xFFFF != drv);
  this->fIO = drv;
}

const UInt16& ATADeviceInterface::GetMaster() {
  return this->fIO;
}

Void ATADeviceInterface::SetMaster(const UInt16& drv) {
  MUST_PASS(0xFFFF != drv);
  this->fMaster = drv;
}