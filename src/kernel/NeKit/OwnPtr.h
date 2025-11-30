
/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <NeKit/Config.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/Ref.h>

namespace Kernel {
template <typename T>
class OwnPtr;

template <typename T>
class NonNullRefPtr;

template <typename T>
class OwnPtr final {
 public:
  OwnPtr() = default;
  ~OwnPtr() { this->Reset(); }

  OwnPtr& operator=(const OwnPtr&) = default;
  OwnPtr(const OwnPtr&)            = default;

 public:
  template <typename... Args>
  bool New(Args&&... arg) {
    if (fCls) {
      return false;
    }

    fCls = new T(arg...);
    return fCls;
  }

  void Reset() {
    if (fCls) delete fCls;
    fCls = nullptr;
  }

  T* operator->() const { return fCls; }

  T* Raw() { return fCls; }

  Ref<T> AsRef() { return Ref<T>(fCls); }

  explicit operator bool() { return fCls; }
  bool     operator!() { return !fCls; }

 private:
  T* fCls{nullptr};
};

template <typename T, typename... Args>
inline OwnPtr<T> make_ptr(Args... args) {
  OwnPtr<T> ret;
  ret.template New<Args...>(forward(args)...);

  return ret;
}
}  // namespace Kernel
