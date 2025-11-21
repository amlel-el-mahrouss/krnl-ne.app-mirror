/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <NeKit/Defines.h>

namespace Kernel {
template <typename T>
class ArrayList final {
 public:
  explicit ArrayList(T* list, SizeT length) : fList(reinterpret_cast<T>(list)), fLen(length) {}

  ~ArrayList() = default;

  ArrayList& operator=(const ArrayList&) = default;
  ArrayList(const ArrayList&)            = default;

  T* Data() { return fList; }

  const T* CData() { return fList; }

  T& operator[](SizeT index) const {
    MUST_PASS(index < this->Count());
    return fList[index];
  }

  operator bool() { return fList; }

  SizeT Count() const { return fLen; }

 private:
  T*    fList{nullptr};
  SizeT fLen{0};
};

template <typename ValueType>
ArrayList<ValueType> make_list(ValueType val) {
  return ArrayList<ValueType>{val};
}
}  // namespace Kernel
