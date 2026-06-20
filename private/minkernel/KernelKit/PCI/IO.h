// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef KERNELKIT_IO_H
#define KERNELKIT_IO_H

#include <ArchKit/ArchKit.h>
#include <NeKit/Array.h>
#include <NeKit/Config.h>
#include <NeKit/Ref.h>

namespace Kernel {
template <SizeT Sz>
class IOArray final {
 public:
  IOArray() = delete;

  IOArray(nullPtr) = delete;

  explicit IOArray(Array<UShort, Sz>& ports) : fPorts(ports) {}

  ~IOArray() {}

  IOArray& operator=(const IOArray&) = default;

  IOArray(const IOArray&) = default;

  operator bool() { return !fPorts.Empty(); }

 public:
  template <typename T>
  T In(SizeT index);

  template <typename T>
  void Out(SizeT index, T value);

 private:
  Array<UShort, Sz> fPorts;
};

inline constexpr UInt16 kMaxPorts = 16;

using IOArray16 = IOArray<kMaxPorts>;

template <SizeT Sz>
inline Array<UShort, Sz> make_ports(UShort base) {
  Array<UShort, Sz> ports;

  for (UShort i = 0; i < Sz; ++i) {
    ports[i] = base + i;
  }

  return ports;
}
}  // namespace Kernel

#ifdef __NE_AMD64__
#include "IOArray+AMD64.inl"
#else
#error Please provide platform specific code for the I/O
#endif  // ifdef __NE_AMD64__

#endif
