// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef __NE_KIT_DOMAIN_H__
#define __NE_KIT_DOMAIN_H__

#include <NeKit/Config.h>

namespace Kernel {
template <class Type>
struct IsDefined final {
  using ResultType    = Type;
  using ResultTypeRef = Type&;
  using TypeRef       = ResultTypeRef;
  using ConstType     = const Type&;
  using TypePtr       = Type*;

  static constexpr bool kValue = true;
};

template <>
struct IsDefined<nullPtr> final {
  static constexpr bool kValue = false;
};

using NullDomain = IsDefined<nullPtr>;

template <class Type>
using Domain = IsDefined<Type>;
}  // namespace Kernel

#endif  // !__NE_KIT_DOMAIN_H__