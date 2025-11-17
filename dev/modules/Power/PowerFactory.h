/* -------------------------------------------

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

------------------------------------------- */

#pragma once

#include <KernelKit/DebugOutput.h>
#include <NeKit/Defines.h>
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