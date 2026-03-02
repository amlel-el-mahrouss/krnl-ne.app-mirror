// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef STORAGEKIT_STORAGEKIT_H
#define STORAGEKIT_STORAGEKIT_H

#define kDriveSectorSizeHDD (512U)
#define kDriveSectorSizeSSD (512U)
#define kDriveSectorSizeOptical (2048U)

namespace Kernel {
template <typename T>
class IDevice;

class NVMEDeviceInterface;
class AHCIDeviceInterface;
class ATADeviceInterface;
class SCSIDeviceInterface;
}  // namespace Kernel

#endif
