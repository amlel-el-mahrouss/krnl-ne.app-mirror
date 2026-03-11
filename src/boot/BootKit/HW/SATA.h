// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel
#ifndef BOOTKIT_SATA_H
#define BOOTKIT_SATA_H

#include <BootKit/BootKit.h>
#include <CompilerKit/CompilerKit.h>
#include <modules/AHCI/AHCI.h>

#define kAHCISectorSz (512)

namespace Boot {
class BootDeviceSATA final {
 public:
  explicit BootDeviceSATA();
  ~BootDeviceSATA() = default;

  NE_COPY_DEFAULT(BootDeviceSATA)

  struct SATATrait final : public Device::Trait {
    Kernel::Boolean mErr{false};
    Kernel::Boolean mDetected{false};

    explicit operator bool() { return !this->mErr; }
  };

  operator bool() { return this->Leak().mDetected; }

  SizeT GetDiskSize() { return drv_std_get_size(); }

  constexpr static auto kSectorSize = kAHCISectorSize;

  BootDeviceSATA& Read(Boot::CharacterTypeASCII* Buf, const Kernel::SizeT SecCount);
  BootDeviceSATA& Write(Boot::CharacterTypeASCII* Buf, const Kernel::SizeT SecCount);

  SATATrait& Leak();

 private:
  SATATrait mTrait;
};
}  // namespace Boot

#endif
