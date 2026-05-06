// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

namespace Kernel {
template <SizeT Sz>
template <typename T>
T IOArray<Sz>::In(SizeT index) {
  switch (sizeof(T)) {
#ifdef __NE_AMD64__
    case 4:
      return HAL::rt_in32(fPorts[index].Leak());
    case 2:
      return HAL::rt_in16(fPorts[index].Leak());
    case 1:
      return HAL::rt_in8(fPorts[index].Leak());
#endif
    default:
      return 0xFFFF;
  }
}

template <SizeT Sz>
template <typename T>
void IOArray<Sz>::Out(SizeT index, T value) {
  switch (sizeof(T)) {
#ifdef __NE_AMD64__
    case 4:
      HAL::rt_out32(fPorts[index].Leak(), value);
    case 2:
      HAL::rt_out16(fPorts[index].Leak(), value);
    case 1:
      HAL::rt_out8(fPorts[index].Leak(), value);
#endif
    default:
      break;
  }
}
}  // namespace Kernel