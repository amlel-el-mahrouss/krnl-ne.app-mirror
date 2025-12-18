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

namespace Kernel::DDK {
inline constexpr auto kInvalidType = 0;

class IDriverBase {
 public:
  explicit IDriverBase() = default;
  virtual ~IDriverBase() = default;

  NE_COPY_DELETE(IDriverBase);
  NE_MOVE_DEFAULT(IDriverBase);

  using PtrType = void*;

  virtual constexpr bool    IsCastable() { return false; }
  virtual constexpr bool    IsActive() { return false; }
  virtual PtrType           Leak() { return nullptr; }
  virtual constexpr int32_t Type() { return kInvalidType; }
};

/// @brief This concept requires the Driver to be IDriverBase compliant.
template <typename Driver>
concept IsValidDriver = requires(Driver driver_base) {
  { driver_base.IsActive() && driver_base.Type() > kInvalidType };
};

/// @brief Consteval helper to detect whether a template is truly based on IDriverBase.
/// @note This helper is consteval only.
template <class Driver>
inline bool ce_ddk_is_valid(Driver drv) {
  return IsValidDriver<Driver>(drv);
}
}  // namespace Kernel::DDK
