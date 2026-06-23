// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef STORAGEKIT_STORAGEKIT_H
#define STORAGEKIT_STORAGEKIT_H

#define kDriveSectorSizeHDD (512U)
#define kDriveSectorSizeSSD (512U)
#define kDriveSectorSizeOptical (2048U)

namespace Ne::Kernel {
template <typename T>
class IDevice;

class NVMEDeviceInterface;
class AHCIDeviceInterface;
class ATADeviceInterface;
class SCSIDeviceInterface;
}  // namespace Ne::Kernel

#endif
