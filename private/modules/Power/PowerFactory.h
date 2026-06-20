// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef POWER_POWERFACTORY_H
#define POWER_POWERFACTORY_H

#include <KernelKit/DebugOutput.h>
#include <NeKit/Config.h>
#include <NeKit/ErrorOr.h>
#include <NeKit/Ref.h>
#include <modules/ACPI/ACPI.h>

#define NE_POWER_FACTORY : public PowerFactory

namespace Kernel {
class PowerFactory {
 public:
  explicit PowerFactory() = default;
  virtual ~PowerFactory() = default;

  PowerFactory& operator=(const PowerFactory&) = default;
  PowerFactory(const PowerFactory&)            = default;

 public:
  virtual Bool Shutdown() { return NO; }  // shutdown
  virtual Void Reboot() {}                // soft-reboot
};
}  // namespace Kernel

#endif
