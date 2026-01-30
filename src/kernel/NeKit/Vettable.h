// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef __NE_KIT_VETTABLE_H__
#define __NE_KIT_VETTABLE_H__

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Config.h>

#define NE_VETTABLE static constexpr BOOL kVettable = YES
#define NE_NON_VETTABLE static constexpr BOOL kVettable = NO

#define NE_UN_VETTABLE static constexpr BOOL kUnvettable = YES

namespace Kernel {
template <class Type>
concept IsVettable = requires(Type) {
  (Type::kVettable);
};

template <class Type>
concept IsUnVettable = requires(Type) {
  (Type::kUnvettable);
};
}  // namespace Kernel

#endif  // !__NE_KIT_VETTABLE_H__
