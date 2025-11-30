
/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Config.h>

#define NE_VETTABLE : public IVet

namespace Kernel {
struct IVet {
  IVet()          = default;
  virtual ~IVet() = default;

  NE_COPY_DEFAULT(IVet)

  operator bool() = delete;
};

template <typename T>
struct Vettable final {
  static constexpr bool kValue = false;
};

template <>
struct Vettable<IVet> final {
  static constexpr bool kValue = true;
};
}  // namespace Kernel