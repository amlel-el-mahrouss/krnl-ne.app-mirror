/* -------------------------------------------

  Copyright (C) 2025, Amlal El Mahrouss, all rights reserved.

------------------------------------------- */

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