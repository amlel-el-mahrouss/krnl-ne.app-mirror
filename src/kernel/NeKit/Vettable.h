
/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef __NE_KIT_VETTABLE_H__
#define __NE_KIT_VETTABLE_H__

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Config.h>

#define NE_VETTABLE static constexpr BOOL kVettable = YES
#define NE_NON_VETTABLE static constexpr BOOL kVettable = NO

namespace Kernel {
template <class Type>
concept IsVettable = requires(Type) {
    (Type::kVettable);
};
}  // namespace Kernel

#endif  // !__NE_KIT_VETTABLE_H__
