
/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Config.h>

#define NE_VETTABLE : public IVet

namespace Kernel {
/// @brief Vet interface for objects.
struct IVet {
  IVet()          = default;
  virtual ~IVet() = default;

  NE_COPY_DEFAULT(IVet)

  /// @brief Start vetting object for validity.
  auto DoVet() const { return false; }
};

template <typename T>
struct Vettable final {
  static constexpr bool kValue = false;
};

template <>
struct Vettable<IVet> final {
  static constexpr bool kValue = true;
};

/// @brief Concept version of Vettable.
template <typename T, typename Fallback>
concept IVettable = requires(IVet vettable, Fallback fallback) {
  { Vettable<T>::kValue ? vettable.DoVet() : fallback(vettable) };
};
}  // namespace Kernel