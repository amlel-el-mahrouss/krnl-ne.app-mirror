// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/ne-kernel

#include <SystemKit/Jail.h>
#include <SystemKit/Syscall.h>

IMPORT_C struct JAIL* JailGetCurrent(Void) {
  return (struct JAIL*) nesys_syscall_arg_1(SYSCALL_HASH("JailGetCurrent"));
}
