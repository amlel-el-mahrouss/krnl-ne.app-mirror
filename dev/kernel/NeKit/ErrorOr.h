/*
 *	========================================================
 *
 *  NeKernel
 * 	Copyright (C) 2024-2025, Amlal El Mahrouss, all rights reserved.
 *
 * 	========================================================
 */

#pragma once

#include <NeKit/Defines.h>
#include <NeKit/Ref.h>

namespace Kernel {
using ErrorT = Int32;

template <typename T>
class ErrorOr final {
 public:
  explicit ErrorOr() = default;
  ~ErrorOr()         = default;

 public:
  explicit ErrorOr(ErrorT err) : mRef((T*) RTL_ALLOCA(sizeof(T))), mId(err) {}

  explicit ErrorOr(nullPtr) {}

  explicit ErrorOr(T* Class) : mRef(Class) {}

  explicit ErrorOr(T Class) : mRef(Class) {}

  ErrorOr& operator=(const ErrorOr&) = default;
  ErrorOr(const ErrorOr&)            = default;

  ErrorOr& operator=(const Ref<T>& refErr) {
    mRef = refErr;
    return *this;
  }

  const T& Value() { return mRef.TryLeak(); }

  Ref<T>& Leak() { return mRef; }

  ErrorT Error() { return mId; }

  /// @note DO NOT MAKE THIS EXPLICIT! IT WILL BREAK THE COMPILATION.
  operator bool() { return mRef; }

  BOOL HasError() { return this->mId > 0; }

 private:
  Ref<T> mRef;
  ErrorT mId{0};
};

using ErrorOrAny = ErrorOr<voidPtr>;

}  // namespace Kernel
