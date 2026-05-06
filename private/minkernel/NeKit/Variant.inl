// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef NEKIT_VARIANT_INL
#define NEKIT_VARIANT_INL

template <typename T>
inline T* Kernel::Variant::As() {
  return reinterpret_cast<T*>(fPtr);
}

#endif
