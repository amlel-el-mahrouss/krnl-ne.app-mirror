// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef KERNELKIT_IFS_H
#define KERNELKIT_IFS_H

#include <KernelKit/DriveMgr.h>

namespace Kernel {

/// @brief Read from IFS disk.
/// @param Mnt mounted interface.
/// @param DrvTrait drive info
/// @param DrvIndex drive index.
/// @return Status code
Int32 fs_ifs_read(IMountpoint* mnt, DriveTrait& drvTrait, Int32 drvIndex);

/// @brief Write to IFS disk.
/// @param Mnt mounted interface.
/// @param DrvTrait drive info
/// @param DrvIndex drive index.
/// @return Status code
Int32 fs_ifs_write(IMountpoint* mnt, DriveTrait& drvTrait, Int32 drvIndex);

}  // namespace Kernel

#endif
