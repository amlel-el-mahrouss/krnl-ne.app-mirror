// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef LIBPOSIX_POSIXKIT_THREAD_H
#define LIBPOSIX_POSIXKIT_THREAD_H

#include <libPOSIX/POSIXKit/unistd.h>

/// @internal Internal type, please do not rely on it for multi-platform builds.
typedef ThreadRef pthread_ref_t;

PTHREAD_SAFE IMPORT_C SInt32        pthread_create(_Output pthread_ref_t* thread, VoidPtr     attr,
                                                   VoidPtr (*start_routine)(VoidPtr), VoidPtr arg);
PTHREAD_SAFE IMPORT_C SInt32        pthread_join(pthread_ref_t thread, VoidPtr* retval);
PTHREAD_SAFE IMPORT_C SInt32        pthread_exit(SInt32 retval);
PTHREAD_SAFE IMPORT_C SInt32        pthread_detach(pthread_ref_t thread);
PTHREAD_SAFE IMPORT_C pthread_ref_t pthread_self(void);
PTHREAD_SAFE IMPORT_C SInt32        pthread_yield(void);

#endif  // LIBPOSIX_POSIXKIT_THREAD_H
