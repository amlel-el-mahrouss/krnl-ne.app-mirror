
/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef __NE_KIT_VETTABLE_H__
#define __NE_KIT_VETTABLE_H__

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Config.h>

namespace Kernel {
template <class Type>
struct Vettable final {
  using ResultType = Type;
  using TypeRef   = Type&;
  using ConstType = const Type&;
  using TypePtr   = Type*;


  static constexpr BOOL kValue = NO;
};

template <class Type>
using FallbackType = Void (*)(const PropertyResult<Type>& type_value);

template <class Type, FallbackType<Type> OnFallback>
concept IsVettable = requires() {
  { Vettable<Type>::kValue ? TrueResult<Type>{} : OnFallback(PropertyResult<Type>{}) };
};
}  // namespace Kernel

#endif  // !__NE_KIT_VETTABLE_H__
