// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <BootKit/BootKit.h>
#include <BootKit/Platform.h>
#include <BootKit/Protocol.h>

#ifdef __BOOTZ_STANDALONE__

/// @brief These functions are used for x64 backends only. Do not try to use the same name for other
/// backends, use their ISA specific conventions.

using namespace Boot;

EXTERN_C void rt_halt() {
  asm volatile("hlt");
}

EXTERN_C void rt_cli() {
  asm volatile("cli");
}

EXTERN_C void rt_sti() {
  asm volatile("sti");
}

EXTERN_C void rt_cld() {
  asm volatile("cld");
}

EXTERN_C void rt_std() {
  asm volatile("std");
}

#endif  // __BOOTZ_STANDALONE__
