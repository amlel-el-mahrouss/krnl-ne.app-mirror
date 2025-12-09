/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <NeKit/Config.h>

namespace Kernel {
template <typename T, SizeT N>
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

  InitializerList& operator=(const InitializerList&) = default;
  InitializerList(const InitializerList&)            = default;

  T*              begin() { return fList; }
  T*              operator->() { return fList; }
  T*              operator*() { return fList; }
  T*              end() { return fList + N; }
  constexpr SizeT size() const { return N; }

 private:
  T fList[N];
};

template <typename ValueType, SizeT N>
inline InitializerList<ValueType, N> make_list(ValueType& val) {
  return InitializerList<ValueType, N>{val};
}
}  // namespace Kernel
