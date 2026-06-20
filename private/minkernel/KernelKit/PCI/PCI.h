// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef KERNELKIT_PCI_H
#define KERNELKIT_PCI_H

#include <NeKit/Config.h>

#define kPCIConfigAddressPort (0xCF8)
#define kPCIConfigDataPort (0xCFC)

#define kPCIDeviceCount (32)
#define kPCIFuncCount (8)
#define kPCIBusCount (256U)

namespace Kernel::PCI {
// model
struct DeviceHeader {
  UInt16 VendorId;
  UInt16 DeviceId;
  UInt8  Command;
  UInt8  Status;
  UInt8  RevisionId;
  UInt8  ProgIf;
  UInt8  SubClass;
  UInt8  Class;
  UInt8  CacheLineSz;
  UInt8  LatencyTimer;
  UInt8  HeaderType;
  UInt8  Bist;
  UInt8  Bus;
  UInt8  Device;
  UInt8  Function;
};

namespace Detail {
  class BAR {
   public:
    UIntPtr BAR;
    SizeT   Size;
  };
}  // namespace Detail

class BAR {
 public:
  Detail::BAR BAR1;
  Detail::BAR BAR2;
  Detail::BAR BAR3;
  Detail::BAR BAR4;
  Detail::BAR BAR5;
};
}  // namespace Kernel::PCI

#endif
