// SPDX-License-Identifier: Apache-2.0
// Copyright 2025-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (See accompanying
// file LICENSE or copy at http://www.apache.org/licenses/LICENSE-2.0)
// Official repository: https://github.com/ne-foss-org/ne_kernel

#include <DriverKit/IO.h>

DDK_EXTERN void kputc(const char ch) {
  if (!ch) return;

  char assembled[2] = {0};
  assembled[0]      = ch;
  assembled[1]      = 0;

  ::ke_call_dispatch("ke_put_string", 2, assembled, 2);
}

/// @brief print string to UART.
/// @param message UART to transmit.
DDK_EXTERN void kprint(const char* message) {
  if (nil == message) return;
  if (*message == '\0') return;

  size_t index = 0;
  size_t len   = ::kstrlen(message);

  while (index < len) {
    ::kputc(message[index]);
    ++index;
  }
}
