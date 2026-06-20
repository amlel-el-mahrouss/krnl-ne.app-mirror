// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef NEKIT_ERROROR_H
#define NEKIT_ERROROR_H

#include <NeKit/Config.h>
#include <NeKit/Ref.h>

namespace Kernel {
/// ================================================================================
/// @brief ErrorOr class for error handling.
/// ================================================================================
template <class T>
class ErrorOr final {
 public:
  ErrorOr()  = default;
  ~ErrorOr() = default;

 public:
  using RefType = Ref<T>;
  using Type    = T;
  using TypePtr = T*;

  explicit ErrorOr(ErrorT err) : mRef(Type()), mId(err) {
    // TODO: Invalidate the value of mRef to make computational evaluations false.
    mRef = nullptr;
  }

  explicit ErrorOr(nullPtr) {}
  explicit ErrorOr(TypePtr klass) : mRef(klass) {}
  explicit ErrorOr(Type klass) : mRef(klass) {}

  ErrorOr& operator=(const ErrorOr&) = default;
  ErrorOr(const ErrorOr&)            = default;

  ErrorOr& operator=(const RefType& ref) {
    mRef = ref;
    return *this;
  }

  const Type& Value() { return mRef.TryLeak(); }

  RefType& Leak() { return mRef; }

  ErrorT Error() { return mId; }

  /// @note DO NOT MAKE THIS EXPLICIT! IT WILL BREAK THE COMPILATION.
  explicit operator bool() { return mRef.Leak(); }

  BOOL HasError() { return this->mId < kErrorSuccess; }

 private:
  RefType mRef;
  ErrorT  mId{0};
};

using ErrorOrAny = ErrorOr<voidPtr>;

}  // namespace Kernel

#endif
