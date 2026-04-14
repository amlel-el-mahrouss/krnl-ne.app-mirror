// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef UTL_BASE_H
#define UTL_BASE_H

#include <math.h>
#include <stddef.h>
#include <stdint.h>

namespace utl {

/**
 * @brief Helper to get number of elements in array.
 *
 * @tparam T Auto-deduced element type
 * @tparam N Auto-deduced number of elements
 * @return Array size
 */
template <class T, size_t N>
constexpr size_t countof(T (&)[N]) {
  return N;
}

}  // namespace utl

#endif