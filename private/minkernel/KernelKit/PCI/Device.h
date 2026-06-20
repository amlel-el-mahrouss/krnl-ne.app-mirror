// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef KERNELKIT_DEVICE_H
#define KERNELKIT_DEVICE_H

#include <NeKit/Config.h>
#include <NeKit/KernelPanic.h>

namespace Kernel::PCI {
enum struct PciConfigKind : UShort {
  ConfigAddress = 0xCF8,
  ConfigData    = 0xCFC,
  CommandReg    = 0x0004,
  Invalid       = 0xFFFF,
};

/// @brief Device interface class
class Device final {
 public:
  Device() = default;

 public:
  Device(UShort bus, UShort device, UShort function, UInt32 bar);

  Device& operator=(const Device&) = default;
  Device(const Device&)            = default;

  ~Device();

 public:
  UInt Read(UInt bar, Size szData);
  void Write(UInt bar, UIntPtr data, Size szData);

 public:
  operator bool();

 public:
  template <typename T>
  UInt Read(UInt bar) {
    STATIC_PASS(sizeof(T) <= sizeof(UInt32), "64-bit PCI addressing is unsupported");
    return Read(bar, sizeof(T));
  }

  template <typename T>
  void Write(UInt bar, UIntPtr data) {
    STATIC_PASS(sizeof(T) <= sizeof(UInt32), "64-bit PCI addressing is unsupported");
    Write(bar, data, sizeof(T));
  }

 public:
  UInt16  DeviceId();
  UShort  VendorId();
  UShort  InterfaceId();
  UChar   Class();
  UChar   Subclass();
  UChar   ProgIf();
  UChar   HeaderType();
  UIntPtr Bar(UInt32 bar_in);

 public:
  void EnableMmio();
  void BecomeBusMaster();  // for PCI-DMA, PC-DMA does not need that.

  UShort Vendor();

 private:
  UShort fBus;
  UShort fDevice;
  UShort fFunction;
  UInt32 fBar;
};
}  // namespace Kernel::PCI

#endif
