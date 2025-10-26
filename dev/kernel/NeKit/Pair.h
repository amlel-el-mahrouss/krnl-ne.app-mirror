/* -------------------------------------------

  Copyright (C) 2024-2025, Amlal El Mahrouss, all rights reserved.

------------------------------------------- */

#pragma once

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Defines.h>
#include <NeKit/ErrorOr.h>

namespace Kernel {
template <typename T1, typename T2>
class Pair;

class PairBuilder;

template <typename T1, typename T2>
class Pair final {
  T1 fFirst{nullptr};
  T2 fSecond{nullptr};

  friend PairBuilder;

 public:
  explicit Pair() = default;
  ~Pair()         = default;

  NE_COPY_DEFAULT(Pair)

  T1& First() { return fFirst; }
  T2& Second() { return fSecond; }

  const T1& First() const { return *fFirst; }
  const T2& Second() const { return *fSecond; }

 private:
  Pair(T1 first, T2 second) : fFirst(first), fSecond(second) {}
};

class PairBuilder final {
  template <typename T1, typename T2>
  STATIC Pair<T1, T2> Construct(T1 first, T2 second) {
    return Pair(first, second);
  }
};

template <typename T1, typename T2>
using PairOr = ErrorOr<Pair<T1, T2>>;
}  // namespace Kernel
