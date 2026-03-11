// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel
#ifndef __BOOTKIT_DEVICE_H__
#define __BOOTKIT_DEVICE_H__

#include <BootKit/Config.h>
#include <modules/AHCI/AHCI.h>
#include <modules/ATA/ATA.h>

namespace Boot {
/// @brief Physical/Virtual device type.
class Device {
 public:
  explicit Device() = default;
  virtual ~Device() = default;

  NE_MOVE_DEFAULT(Device)

  struct Trait {
    Lba   mBase{0};
    SizeT mSize{0};
  };

  virtual Trait& Leak() = 0;

  virtual Device& Read(Char* Buf, SizeT SecCount)  = 0;
  virtual Device& Write(Char* Buf, SizeT SecCount) = 0;
};

using BootDevice    = Device;
using NetworkDevice = Device;
using DiskDevice    = Device;
}  // namespace Boot

#endif