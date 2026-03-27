// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef FW_HEADERS_ATOM_H
#define FW_HEADERS_ATOM_H

#include <CoreFoundation.fwrk/headers/Foundation.h>

namespace CF {
template <class T>
class CFAtom final {
 public:
  explicit CFAtom() = default;
  ~CFAtom()         = default;

 public:
  CFAtom& operator=(const CFAtom&) = delete;
  CFAtom(const CFAtom&)            = delete;

 public:
  const T& operator[](const T& bit) { return (fArrayOfAtoms & (T{1} << bit)); }
  void     operator|(const T& bit) { fArrayOfAtoms |= (T{1} << bit); }

  friend Boolean operator==(CFAtom<T>& atomic, const T& idx) { return atomic[idx] == idx; }

  friend Boolean operator!=(CFAtom<T>& atomic, const T& idx) { return atomic[idx] == idx; }

 private:
  T fArrayOfAtoms;
};
}  // namespace CF

#endif
