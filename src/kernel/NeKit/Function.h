// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef NEKIT_FUNCTION_H
#define NEKIT_FUNCTION_H

#include <NeKit/Config.h>
#include <NeKit/ErrorOr.h>

namespace Kernel {

/// @brief Function Pointer Container.
template <typename T, typename... Args>
class Function final {
 public:
  Function()        = delete;
  Function(nullPtr) = delete;

  ~Function() = default;
  Function(T (*Fn)(Args... args)) : fFn(Fn) { MUST_PASS(fFn); }

  Function& operator=(const Function&) = delete;
  Function(const Function&)            = delete;

  T operator()(Args&&... args) { return fFn(forward<Args...>(args)...); }

  explicit operator bool() { return fFn; }
  bool     operator!() { return !fFn; }

 private:
  T (*fFn)(Args... args){nullptr};
};

template <typename T, typename... Args>
using FunctionOr = ErrorOr<Function<T, Args...>>;

template <typename T, typename... Args>
using FunctionRef = Ref<Function<T, Args...>>;

}  // namespace Kernel

#endif
