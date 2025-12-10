/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <NeKit/Config.h>
#include <NeKit/ErrorOr.h>

namespace Kernel {
template <class T, SizeT N>
class InitializerList final {
 public:
  explicit InitializerList(const T* list) {
    if constexpr (N > 0) {
      for (auto i = 0UL; i < N; ++i) {
        fList[i] = list[i];
      }
    }
  }

  ~InitializerList() = default;

  InitializerList& operator=(const InitializerList&) = delete;
  InitializerList(const InitializerList&)            = delete;

  T*              begin() { return fList; }
  T*              end() { return fList + N; }
  constexpr SizeT size() const { return N; }
  
  T*              operator->() { return fList; }
  T*              operator*() { return fList; }

 private:
  T fList[N];
};

template <class T, SizeT N>
using ErrorOrList = ErrorOr<InitializerList<T, N>>;
}  // namespace Kernel
