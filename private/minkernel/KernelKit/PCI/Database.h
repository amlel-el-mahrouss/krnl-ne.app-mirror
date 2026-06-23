// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef KERNELKIT_DATABASE_H
#define KERNELKIT_DATABASE_H

#include <KernelKit/PCI/Device.h>
#include <NeKit/Config.h>

namespace Ne::Kernel {
namespace Types {
  // https://wiki.osdev.org/PCI
  enum struct PciDeviceKind : UChar {
    MassStorageController             = 0x1,
    NetworkController                 = 0x2,
    DisplayController                 = 0x3,
    MultimediaController              = 0x4,
    MemoryController                  = 0x5,
    Bridge                            = 0x6,
    CommunicationController           = 0x7,
    GenericSystemPeripheral           = 0x8,
    InputDeviceController             = 0x9,
    DockingStation                    = 0xa,
    Processor                         = 0xb,
    SerialBusController               = 0xc,
    WirelessController                = 0xd,
    IntelligentController             = 0xe,
    SatelliteCommunicationsController = 0xf,
    CoProcessor                       = 0x40,
    Unassgined                        = 0xf,
    Invalid                           = Unassgined,
  };
}  // namespace Types
}  // namespace Ne::Kernel

inline BOOL operator!=(const Ne::Kernel::Types::PciDeviceKind& lhs, Ne::Kernel::UChar rhs) {
  return rhs != (Ne::Kernel::UChar) lhs;
}

inline BOOL operator==(const Ne::Kernel::Types::PciDeviceKind& lhs, Ne::Kernel::UChar rhs) {
  return rhs == (Ne::Kernel::UChar) lhs;
}

inline BOOL operator!=(Ne::Kernel::UChar lhs, const Ne::Kernel::Types::PciDeviceKind& rhs) {
  return lhs != (Ne::Kernel::UChar) rhs;
}

inline BOOL operator==(Ne::Kernel::UChar lhs, const Ne::Kernel::Types::PciDeviceKind& rhs) {
  return lhs == (Ne::Kernel::UChar) rhs;
}

#endif
