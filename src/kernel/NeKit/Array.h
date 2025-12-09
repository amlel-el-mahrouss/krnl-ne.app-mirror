/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <KernelKit/DebugOutput.h>
#include <NeKit/Config.h>
#include <NeKit/ErrorOr.h>

namespace Kernel {
template <typename T, SizeT N>
class Array final {
 public:
  explicit Array() = default;
  ~Array()         = default;

  Array& operator=(const Array&) = default;
  Array(const Array&)            = default;

  T&       operator[](const SizeT& at) { return fArray[at]; }
  const T& operator[](const SizeT& at) const { return fArray[at]; }

  Boolean Empty() { return this->Count() > 0; }

  SizeT Capacity() { return N; }

  SizeT Count() { return N; }

  const T* CData() { return fArray; }

  explicit operator bool() { return !this->Empty(); }

 private:
  T fArray[N];
};

template <typename ValueType>
inline auto make_array(ValueType& val) -> auto {
  return Array<ValueType, ARRAY_SIZE(val)>{val};
}
}  // namespace Kernel
