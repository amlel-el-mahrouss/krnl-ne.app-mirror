// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef NEKIT_OWNPTR_H
#define NEKIT_OWNPTR_H

#include <NeKit/Config.h>
#include <NeKit/ErrorOr.h>
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
  OwnPtr() : fCls(nullptr) {}
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
    return fCls != nullptr;
  }

  void Reset() {
    if (fCls) delete fCls;
    fCls = nullptr;
  }

  T* operator->() const { return fCls; }

  T* Raw() { return fCls; }

  Ref<T> AsRef() { return Ref<T>(fCls); }

  explicit operator bool() { return fCls; }

 private:
  T* fCls{nullptr};
};

template <typename T>
using OwnOr = ErrorOr<OwnPtr<T>>;

template <typename T, typename... Args>
inline OwnPtr<T> make_ptr(Args&&... args) {
  OwnPtr<T> ret;
  ret.template New<Args...>(forward(args)...);

  return ret;
}
}  // namespace Kernel

#endif