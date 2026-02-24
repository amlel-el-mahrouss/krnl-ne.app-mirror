// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef DRIVERKIT_OBJECTKIT_DRIVER_BASE_H
#define DRIVERKIT_OBJECTKIT_DRIVER_BASE_H

#include <libDDK/DriverKit/macros.h>

#define DDK_DRIVER_IMPL \
  final:                \
  public                \
  ::Kernel::DDK::IDriverBase

/// @author Amlal El Mahrouss
/// @brief The DDK C++ API.

namespace Kernel::DDK {

inline constexpr auto kInvalidType = 0;

/// @brief Driver interface type.
class IDriverBase {
 public:
  IDriverBase()          = default;
  virtual ~IDriverBase() = default;

  IDriverBase& operator=(const IDriverBase&) = default;
  IDriverBase(const IDriverBase&)            = default;

  using PtrType = void*;

  virtual constexpr bool    IsCastable() { return false; }
  virtual constexpr bool    IsActive() { return false; }
  virtual PtrType           Leak() { return nullptr; }
  virtual constexpr int32_t Type() { return kInvalidType; }
};

/// @brief This concept requires the Driver to be IDriverBase compliant.
/// @author @amlel-el-mahrouss
template <typename Driver>
concept IsValidDriver = requires(Driver drv) {
  { drv.IsActive() && drv.Type() > kInvalidType };
};

}  // namespace Kernel::DDK

#endif
