// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef LIBPOSIX_POSIXKIT_POSIX_H
#define LIBPOSIX_POSIXKIT_POSIX_H

#include <SystemKit/System.h>

/// @file POSIX.h
/// @brief POSIX definitions header for the NeKernel.

/// @brief Please use these macros to specify whether your function is thread safe or not.
#define PTHREAD_UNSAFE __THREAD_UNSAFE
#define PTHREAD_SAFE __THREAD_SAFE

#define _POSIX_SOURCE __POSIX_SOURCE__
#define _XOPEN_SOURCE __XOPEN_SOURCE__

PTHREAD_UNSAFE IMPORT_C SInt64 write(SizeT count, SInt32 fd, Void* data, SizeT sz);
PTHREAD_UNSAFE IMPORT_C SInt64 read(SizeT count, SInt32 fd, Void* data, SizeT sz);

#endif  // LIBPOSIX_POSIXKIT_POSIX_H
