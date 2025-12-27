// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef HEADERS_ATOM_H
#define HEADERS_ATOM_H

#include <CoreFoundation.fwrk/headers/Foundation.h>

namespace CF {
template <typename T>
class CFAtom final {
 public:
  explicit CFAtom() = default;
  ~CFAtom()         = default;

 public:
  CFAtom& operator=(const CFAtom&) = delete;
  CFAtom(const CFAtom&)            = delete;

 public:
  T operator[](SizeT bit) { return (fArrayOfAtoms & (1 << bit)); }

  void operator|(SizeT bit) { fArrayOfAtoms |= (1 << bit); }

  friend Boolean operator==(CFAtom<T>& atomic, const T& idx) { return atomic[idx] == idx; }

  friend Boolean operator!=(CFAtom<T>& atomic, const T& idx) { return atomic[idx] == idx; }

 private:
  T fArrayOfAtoms;
};
}  // namespace CF

#endif
