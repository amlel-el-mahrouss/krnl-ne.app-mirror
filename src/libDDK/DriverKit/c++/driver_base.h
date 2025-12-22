/* ========================================

   Copyright Amlal El Mahrouss 2025, licensed under the Apache 2.0 license.

   FILE: driver_base.h
   PURPOSE: C++ Driver Wrapper.

   ======================================== */

#pragma once

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

  NE_COPY_DELETE(IDriverBase);

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
