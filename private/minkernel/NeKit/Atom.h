// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef NEKIT_ATOM_H
#define NEKIT_ATOM_H

#include <NeKit/Config.h>

namespace Kernel {

template <class AtomicType>
class Atom final {
 public:
  explicit Atom() = default;
  ~Atom()         = default;

 public:
  Atom& operator=(const Atom&) = delete;
  Atom(const Atom&)            = delete;

 public:
  using Type     = AtomicType;
  using Ref      = AtomicType&;
  using ConstRef = const AtomicType&;

  const AtomicType& operator[](const AtomicType& bit) {
    return (fArrayOfAtoms & (AtomicType{} << bit));
  }
  void operator|(const AtomicType& bit) { fArrayOfAtoms |= (AtomicType{1} << bit); }

  Atom& operator|=(const AtomicType& bit) {
    this->operator|(bit);
    return *this;
  }

  friend bool operator==(Atom<AtomicType>& atomic, const AtomicType& idx) {
    return atomic[idx] == idx;
  }
  friend bool operator!=(Atom<AtomicType>& atomic, const AtomicType& idx) {
    return atomic[idx] != idx;
  }

 private:
  AtomicType fArrayOfAtoms;
};

}  // namespace Kernel

#endif
