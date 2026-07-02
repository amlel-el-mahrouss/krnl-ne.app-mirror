// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef LIBPOSIX_POSIXKIT_THREAD_H
#define LIBPOSIX_POSIXKIT_THREAD_H

#include <POSIXKit/unistd.h>

/// @internal Internal type, please do not rely on it for multi-platform builds.
typedef ThreadRef pthread_ref_t;

IMPORT_C PTHREAD_SAFE SInt32        pthread_create(_Output pthread_ref_t* thread, VoidPtr     attr,
                                                   VoidPtr (*start_routine)(VoidPtr), VoidPtr arg);
IMPORT_C PTHREAD_SAFE SInt32        pthread_join(pthread_ref_t thread, VoidPtr* retval);
IMPORT_C PTHREAD_SAFE SInt32        pthread_exit(SInt32 retval);
IMPORT_C PTHREAD_SAFE SInt32        pthread_detach(pthread_ref_t thread);
IMPORT_C PTHREAD_SAFE pthread_ref_t pthread_self(void);
IMPORT_C PTHREAD_SAFE SInt32        pthread_yield(void);

#endif  // LIBPOSIX_POSIXKIT_THREAD_H
