// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef KERNELKIT_DATABASE_H
#define KERNELKIT_DATABASE_H

#include <KernelKit/PCI/Device.h>
#include <NeKit/Config.h>

namespace Kernel {
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
}  // namespace Kernel

inline BOOL operator!=(const Kernel::Types::PciDeviceKind& lhs, Kernel::UChar rhs) {
  return rhs != (Kernel::UChar) lhs;
}

inline BOOL operator==(const Kernel::Types::PciDeviceKind& lhs, Kernel::UChar rhs) {
  return rhs == (Kernel::UChar) lhs;
}

inline BOOL operator!=(Kernel::UChar lhs, const Kernel::Types::PciDeviceKind& rhs) {
  return lhs != (Kernel::UChar) rhs;
}

inline BOOL operator==(Kernel::UChar lhs, const Kernel::Types::PciDeviceKind& rhs) {
  return lhs == (Kernel::UChar) rhs;
}

#endif
