// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#include <libSystem/SystemKit/Err.h>
#include <libSystem/SystemKit/Syscall.h>
#include <libSystem/SystemKit/Verify.h>

using namespace LibSystem;

IMPORT_C Void _rtl_assert(Bool expr, const Char* origin) {
  if (!expr) {
    PrintOut(nullptr, "Assertion failed: %s\r", origin);
    libsys_syscall_arg_1(SYSCALL_HASH("_rtl_debug_break"));
  }
}