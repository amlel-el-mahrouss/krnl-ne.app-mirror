
/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef __NE_KIT_VETTABLE_H__
#define __NE_KIT_VETTABLE_H__

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Config.h>

#define NE_VETTABLE \
  final:            \
  public            \
  ::Kernel::IVettable

#define NE_NOT_VETTABLE \
  final:                \
  public                \
  ::Kernel::INotVettable

namespace Kernel {
struct IVettable {
  explicit IVettable() = default;
  virtual ~IVettable() = default;

  NE_COPY_DEFAULT(IVettable)
};

template <class Type>
struct Vettable final {
  static constexpr bool kValue = false;
};

template <>
struct Vettable<IVettable> final {
  static constexpr bool kValue = true;
};

template <class Type>
using FallbackType = Void (*)(const PropertyResult<Type>& type_value);

template <class Type, FallbackType<Type> OnFallback>
concept IsVettable = requires() {
  { Vettable<Type>::kValue ? TrueResult<Type>{} : OnFallback(PropertyResult<Type>{}) };
};
}  // namespace Kernel

#endif  // !__NE_KIT_VETTABLE_H__