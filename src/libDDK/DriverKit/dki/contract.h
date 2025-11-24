/* ========================================

   Copyright Amlal El Mahrouss 2025, licensed under the Apache 2.0 license.

   FILE: ddk.h
   PURPOSE: Driver Kernel Interface Model base header.

   ======================================== */

#pragma once

#include <CompilerKit/CompilerKit.h>
#include <libDDK/DriverKit/macros.h>

#define DKI_CONTRACT_IMPL final : public ::Kernel::DKI::DKIContract

/// @author Amlal El Mahrouss

namespace Kernel::DKI {
class DKIContract {
 public:
  explicit DKIContract() = default;
  virtual ~DKIContract() = default;

  NE_COPY_DEFAULT(DKIContract);

  using PtrType = VoidPtr;

  virtual BOOL    IsCastable() { return NO; }
  virtual BOOL    IsActive() { return NO; }
  virtual VoidPtr Leak() { return nullptr; }
  virtual Int32   Type() { return 0; }
};
}  // namespace Kernel::DKI
