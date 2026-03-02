// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef SYSTEMKIT_SYSCALL_H
#define SYSTEMKIT_SYSCALL_H

#include <libSystem/SystemKit/System.h>
#include <cstdarg>

IMPORT_C VoidPtr libsys_syscall_arg_1(SizeT id);
IMPORT_C VoidPtr libsys_syscall_arg_2(SizeT id, VoidPtr arg1);
IMPORT_C VoidPtr libsys_syscall_arg_3(SizeT id, VoidPtr arg1, VoidPtr arg3);
IMPORT_C VoidPtr libsys_syscall_arg_4(SizeT id, VoidPtr arg1, VoidPtr arg3, VoidPtr arg4);

IMPORT_C UInt64 libsys_hash_64(const Char* path);

#ifndef SYSCALL_HASH
#define SYSCALL_HASH(str) libsys_hash_64(str)
#endif  // !SYSCALL_HASH

#endif
