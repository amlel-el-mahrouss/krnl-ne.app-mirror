
/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef __NE_KIT_VETTABLE_H__
#define __NE_KIT_VETTABLE_H__

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Config.h>

#define NE_VETTABLE : public ::Kernel::IVettable

namespace Kernel {
/// @brief Vet interface for objects.
struct IVettable {
  explicit IVettable() = default;
  virtual ~IVettable() = default;

  NE_COPY_DEFAULT(IVettable)
};

struct INotVettable {
  explicit INotVettable() = default;
  virtual ~INotVettable() = default;

  NE_COPY_DEFAULT(INotVettable)
};

template <typename T>
struct Vettable final {
  static constexpr bool kValue = false;
};

template <>
struct Vettable<INotVettable> final {
  static constexpr bool kValue = false;
};

template <>
struct Vettable<IVettable> final {
  static constexpr bool kValue = true;
};

/// @brief Concept version of Vettable.
template <typename T, typename OnFallback>
concept IsVettable = requires(OnFallback fallback) {
  { Vettable<T>::kValue ? true : fallback() };
};

template <typename T, typename OnFallback>
concept IsNotVettable = requires(OnFallback fallback) {
  { !Vettable<T>::kValue ? true : fallback() };
};
}  // namespace Kernel

#endif  // !__NE_KIT_VETTABLE_H__