/* -------------------------------------------

  Copyright (C) 2025, Amlal El Mahrouss, all rights reserved.

------------------------------------------- */

#include <libSystem/SystemKit/Err.h>
#include <libSystem/SystemKit/Syscall.h>
#include <libSystem/SystemKit/System.h>
#include <libSystem/SystemKit/Jail.h>
#include <libSystem/SystemKit/Verify.h>

using namespace LibSystem;

IMPORT_C struct JAIL* JailGetCurrent(Void) {
    return (struct JAIL*)libsys_syscall_arg_1(SYSCALL_HASH("JailGetCurrent"));
}