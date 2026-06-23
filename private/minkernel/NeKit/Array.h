// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef NEKIT_ARRAY_H
#define NEKIT_ARRAY_H

#include <KernelKit/DebugOutput.h>
#include <NeKit/Config.h>
#include <NeKit/ErrorOr.h>

namespace Ne::Kernel {

template <class T, SizeT N>
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

template <class ValueType>
inline auto make_array(ValueType& val) -> auto {
  return Array<ValueType, ARRAY_SIZE(val)>{val};
}

}  // namespace Ne::Kernel

#endif
