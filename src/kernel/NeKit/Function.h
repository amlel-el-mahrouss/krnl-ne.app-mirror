/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <NeKit/Config.h>
#include <NeKit/ErrorOr.h>

namespace Kernel {
/// ================================================================================
/// @brief Function wrapper class.
/// ================================================================================
template <typename T, typename... Args>
class Function final {
 public:
  Function() = delete;
  Function(nullPtr) = delete;
  ~Function() = default;

  explicit Function(T (*Fn)(Args... args)) : fFn(Fn) { MUST_PASS(fFn); }

  Function& operator=(const Function&) = delete;
  Function(const Function&)            = delete;

  T operator()(Args&&... args) {
    return fFn(args...);
  }

  T Call(Args&&... args) {
    return fFn(args...);
  }

  explicit operator bool() { return fFn; }
  bool operator!() { return !fFn; }

 private:
  T (*fFn)(Args... args){nullptr};
};

template <typename T, typename... Args>
using FunctionOr = ErrorOr<Function<T, Args...>>;
}  // namespace Kernel
