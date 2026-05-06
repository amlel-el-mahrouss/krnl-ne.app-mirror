// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef _NEKIT_REF_H_
#define _NEKIT_REF_H_

#include <CompilerKit/CompilerKit.h>
#include <KernelKit/HeapMgr.h>
#include <NeKit/Config.h>
#include <NeKit/Domain.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/Vettable.h>

namespace Kernel {

/// @brief Reference wrapper class.
template <typename T>
class Ref final {
 public:
  Ref()  = default;
  ~Ref() = default;

 public:
  using Type      = T;
  using ConstType = const T&;
  using RefType   = T&;

  Ref(Type cls) : fClass(cls) {}

  Ref& operator=(nullPtr) { return *this; }

  Ref& operator=(Type* ref) {
    fClass = *ref;
    return *this;
  }

  Ref& operator=(Type ref) {
    fClass = ref;
    return *this;
  }

  NE_COPY_DEFAULT(Ref)

 public:
  Type operator->() const { return fClass; }

  Type& Leak() { return fClass; }

  Type& TryLeak() {
    MUST_PASS(fClass);
    return fClass;
  }

  Type operator*() { return fClass; }

  explicit operator bool() {
    if constexpr (IsVettable<Type>) {
      return Type::kVettable;
    } else {
      return false;
    }
  }

 private:
  Type fClass;
};

/// @brief Non Nullable Reference class.
template <typename T>
class NonNullRef final {
 public:
  using RefType = Ref<T>;
  using Type    = T;

  NonNullRef() = delete;

  NonNullRef(Type* ref) : fRef(ref) {}
  NonNullRef(nullPtr ref) = delete;
  NonNullRef(RefType ref) : fRef(ref) {}

  Ref<T>& operator->() {
    MUST_PASS(fRef);
    return fRef;
  }

  NonNullRef& operator=(const NonNullRef<T>& ref) = delete;
  NonNullRef(const NonNullRef<T>& ref)            = delete;

 private:
  Ref<T> fRef{};
};

using RefAny        = Ref<Any>;
using NonNullRefAny = NonNullRef<Any>;

}  // namespace Kernel

#endif  // ifndef _NEKIT_REF_H_
