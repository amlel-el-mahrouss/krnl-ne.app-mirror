/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#include <KernelKit/PCI/Iterator.h>

namespace Kernel::PCI {
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
}  // namespace Kernel::PCI
