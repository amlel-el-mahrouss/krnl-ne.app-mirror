/* ========================================

   Copyright Amlal El Mahrouss 2025, licensed under the Apache 2.0 license.

   FILE: driver_base.h
   PURPOSE: IDriverBase and friends.

   ======================================== */

#pragma once

#include <CompilerKit/CompilerKit.h>
#include <libDDK/DriverKit/macros.h>

#define DKI_DRIVER_IMPL \
  final:                \
  public                \
  ::Kernel::DDK::IDriverBase

/// @author Amlal El Mahrouss

namespace Kernel::DDK {
class IDriverBase {
 public:
  explicit IDriverBase() = default;
  virtual ~IDriverBase() = default;

  NE_COPY_DEFAULT(IDriverBase);

  using PtrType = VoidPtr;

  virtual BOOL            IsCastable() { return NO; }
  virtual constexpr BOOL  IsActive() { return NO; }
  virtual PtrType         Leak() { return nullptr; }
  virtual constexpr Int32 Type() { return 0; }
};

/// @brief This concept requires the Driver to be IDriverBase compliant.
template <typename T>
concept IsValidDriver = requires(T a) {
  { a.IsActive() && a.Type() > 0 };
};

/// @brief Consteval helper to detect whether a template is truly based on IDriverBase.
/// @note This helper is consteval only.
template <IsValidDriver T>
inline consteval void ce_ddk_is_valid(T) {}
}  // namespace Kernel::DDK
