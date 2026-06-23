// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <KernelKit/PCI/Iterator.h>

namespace Ne::Kernel::PCI {
Iterator::Iterator(const Types::PciDeviceKind type, UInt32 bar) {
  // probe devices.
  for (Int32 bus = 0; bus < NE_BUS_COUNT; ++bus) {
    for (Int32 device = 0; device < NE_DEVICE_COUNT; ++device) {
      for (Int32 function = 0; function < NE_FUNCTION_COUNT; ++function) {
        Device dev(bus, device, function, bar);

        if (dev.Class() == type) {
          fDevices[bus] = dev;
        }
      }
    }
  }
}

Iterator::~Iterator() {}

Ref<PCI::Device> Iterator::operator[](const Size& at) {
  return fDevices[at];
}
}  // namespace Ne::Kernel::PCI
