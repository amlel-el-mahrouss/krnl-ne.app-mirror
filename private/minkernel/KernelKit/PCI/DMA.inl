// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

namespace Kernel {
template <class T>
T* DMAWrapper::operator->() {
  return this->fAddress;
}

template <class T>
T* DMAWrapper::Get(UIntPtr offset) {
  return reinterpret_cast<T*>((UIntPtr) this->fAddress + offset);
}
}  // namespace Kernel
