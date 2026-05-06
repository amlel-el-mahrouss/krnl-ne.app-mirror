// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef __PCI_ITERATOR_H__
#define __PCI_ITERATOR_H__

#include <KernelKit/PCI/Database.h>
#include <KernelKit/PCI/Device.h>
#include <NeKit/Array.h>
#include <NeKit/Config.h>
#include <NeKit/Ref.h>

#define NE_BUS_COUNT (256)
#define NE_DEVICE_COUNT (33)
#define NE_FUNCTION_COUNT (8)

namespace Kernel::PCI {
class Iterator final {
 public:
  Iterator() = delete;

 public:
  explicit Iterator(const Types::PciDeviceKind deviceType, UInt32 bar);

  Iterator& operator=(const Iterator&) = default;
  Iterator(const Iterator&)            = default;

  ~Iterator();

 public:
  Ref<PCI::Device> operator[](const Size& sz);

 private:
  Array<PCI::Device, NE_BUS_COUNT> fDevices;
};
}  // namespace Kernel::PCI

#endif  // __PCI_ITERATOR_H__
