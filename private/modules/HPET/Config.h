// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef HPET_CONFIG_H
#define HPET_CONFIG_H

#include <NeKit/Config.h>
#include <modules/ACPI/ACPI.h>

namespace Ne::Kernel {
  
struct PACKED HPETAddressStructure final {
  Ne::Kernel::UInt8  AddressSpaceId;  // 0 - system memory, 1 - system I/O
  Ne::Kernel::UInt8  RegisterBitWidth;
  Ne::Kernel::UInt8  RegisterBitOffset;
  Ne::Kernel::UInt8  Reserved;
  Ne::Kernel::UInt64 Address;
};

struct PACKED HPETHeader final : public SDT {
  Ne::Kernel::UInt8        HardwareRevId;
  Ne::Kernel::UInt8        ComparatorCount : 5;
  Ne::Kernel::UInt8        CounterSize : 1;
  Ne::Kernel::UInt8        Reserved : 1;
  Ne::Kernel::UInt8        LegacyReplacement : 1;
  Ne::Kernel::UInt16       PciVendorId;
  HPETAddressStructure Address;
  Ne::Kernel::UInt8        HpetNumber;
  Ne::Kernel::UInt16       MinimumTick;
  Ne::Kernel::UInt8        PageProtection;
};

}  // namespace Ne::Kernel

#endif
