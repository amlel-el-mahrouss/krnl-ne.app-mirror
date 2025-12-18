/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef __NE_KIT_INIT_LIST_H__
#define __NE_KIT_INIT_LIST_H__

#include <NeKit/Config.h>
#include <NeKit/ErrorOr.h>

namespace Kernel {
// \brief Initalizer List object for containers.
template <class Type, SizeT N>
class InitializerList final {
 public:
  InitializerList() = delete;

  explicit InitializerList(const Type* list) {
    if constexpr (N > 0) {
      for (auto i = 0UL; i < N; ++i) {
        fList[i] = list[i];
      }
    }
  }

  ~InitializerList() = default;

  InitializerList& operator=(const InitializerList&) = delete;
  InitializerList(const InitializerList&)            = delete;

  Type* begin() { return fList; }
  Type* end() { return fList + N; }

  constexpr SizeT size() const { return N; }

  Type* operator->() { return this->begin(); }
  Type* operator*() { return this->begin(); }

 private:
  Type fList[N];
};

template <class T, SizeT N>
using ErrorOrList = ErrorOr<InitializerList<T, N>>;
}  // namespace Kernel

#endif
