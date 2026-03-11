// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel
#ifndef BOOTKIT_ATA_H
#define BOOTKIT_ATA_H

#include <BootKit/Device.h>
#include <modules/ATA/ATA.h>

namespace Boot {
class BootDeviceATA final : public Device {
 public:
  enum {
    kPrimary   = ATA_PRIMARY_IO,
    kSecondary = ATA_SECONDARY_IO,
  };

  explicit BootDeviceATA();
  ~BootDeviceATA() = default;

  enum { kSectorSize = kATASectorSize };

  struct ATATrait final : public Device::Trait {
    UInt16  mBus{kPrimary};
    UInt8   mMaster{0};
    Boolean mErr{false};

    operator bool() { return !mErr; }
  };

 public:
  operator bool();

  SizeT GetSectorsCount();
  SizeT GetDiskSize();

  BootDeviceATA& Read(Char* Buf, SizeT SecCount) override;
  BootDeviceATA& Write(Char* Buf, SizeT SecCount) override;

  ATATrait& Leak() override;

 private:
  ATATrait mTrait;
};
}  // namespace Boot

#endif
