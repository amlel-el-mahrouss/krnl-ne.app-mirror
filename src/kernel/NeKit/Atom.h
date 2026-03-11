// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef NEKIT_ATOM_H
#define NEKIT_ATOM_H

#include <NeKit/Config.h>

namespace Kernel {

template <class TypeAtomic>
class Atom final {
 public:
  explicit Atom() = default;
  ~Atom()         = default;

 public:
  Atom& operator=(const Atom&) = delete;
  Atom(const Atom&)            = delete;

 public:
  using Type     = TypeAtomic;
  using Ref      = TypeAtomic&;
  using ConstRef = const TypeAtomic&;

  const TypeAtomic& operator[](const SizeT& bit) { return (fArrayOfAtoms & (1 << bit)); }

  void operator|(const SizeT& bit) { fArrayOfAtoms |= (1 << bit); }

  Atom& operator|=(const SizeT& bit) {
    this->operator|(bit);
    return *this;
  }

  friend bool operator==(Atom<TypeAtomic>& atomic, const TypeAtomic& idx) {
    return atomic[idx] == idx;
  }
  friend bool operator!=(Atom<TypeAtomic>& atomic, const TypeAtomic& idx) {
    return atomic[idx] != idx;
  }

 private:
  TypeAtomic fArrayOfAtoms;
};

}  // namespace Kernel

#endif
