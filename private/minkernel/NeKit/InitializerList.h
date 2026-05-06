// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

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
      for (auto i{0ull}; i < N; ++i) {
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
