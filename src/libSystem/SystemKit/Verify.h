/* ========================================

   Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

   File: Verify.h
   Purpose: System Call Interface Verification Layer.

   ======================================== */

#ifndef SYSTEMKIT_VERIFY_H
#define SYSTEMKIT_VERIFY_H

#include <libSystem/SystemKit/System.h>

namespace LibSystem::Verify {
/// @author 0xf00sec, and Amlal El Mahrouss
/// @brief safe cast operator.
template <class T, class R>
inline R* sys_safe_cast(const T* ptr) {
  ::_rtl_assert(ptr, "safe cast failed!");
  return static_cast<R*>(const_cast<T*>(ptr));
}

template <class T, class U>
struct is_castable {
  using return_type = U;

  constexpr static BOOL value = false;
};

template <class T>
struct is_castable<T, T> {
  using return_type = T;

  constexpr static BOOL value = true;
};

/// @author Amlal El Mahrouss
/// @brief Safe constexpr cast.
template <typename T, typename R>
constexpr R* sys_constexpr_cast(T* ptr) {
  static_assert(is_castable<T, R>::value, "types cannot be casted.");
  return static_cast<R*>(ptr);
}
}  // namespace LibSystem::Verify

#endif
