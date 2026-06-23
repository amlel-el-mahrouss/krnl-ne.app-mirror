// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <ArchKit/ArchKit.h>
#include <KernelKit/DebugOutput.h>
#include <NeKit/New.h>
#include <NeKit/Utils.h>

namespace Ne::Kernel {
EXTERN_C void ke_io_write(IDevice<const Char*>* self, const Char* bytes) {
#ifdef __DEBUG__
  if (*bytes == 0) return;

  SizeT index = 0;
  SizeT len   = 0;

  index = 0;
  len   = rt_string_len(bytes, 256U);

  volatile UInt8* uart_ptr = (UInt8*) 0x09000000;

  while (index < len) {
    if (bytes[index] == '\r') *uart_ptr = '\r';

    *uart_ptr = bytes[index] == '\r' ? '\n' : bytes[index];
    ++index;
  }
#endif  // __DEBUG__
}

TerminalDevice::~TerminalDevice() = default;

EXTERN_C void ke_io_read(IDevice<const Char*>* self, const Char* bytes) {
#ifdef __DEBUG__
  volatile UInt8* uart_ptr = (UInt8*) 0x09000000;
  Char* in  = (Char*) bytes;
  auto  in_ex  = *uart_ptr;
  auto index = 0;
  in[index] = in_ex;
#endif  // __DEBUG__
}

TerminalDevice TerminalDevice::The() {
  TerminalDevice out(Ne::Kernel::ke_io_write, Ne::Kernel::ke_io_read);
  return out;
}

}  // namespace Ne::Kernel
