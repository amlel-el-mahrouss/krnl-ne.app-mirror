// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <HALKit/POWER/Processor.h>
#include <KernelKit/DebugOutput.h>

using namespace Kernel;

/// @brief Writes to COM1.
/// @param bytes
void ke_io_write(const Char* bytes) {
  if (!bytes) return;

  SizeT index = 0;
  SizeT len   = rt_string_len(bytes, 256U);

  while (index < len) {
    // TODO
    ++index;
  }
}
