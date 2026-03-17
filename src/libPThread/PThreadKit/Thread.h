// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef LIBPOSIX_POSIXKIT_THREAD_H
#define LIBPOSIX_POSIXKIT_THREAD_H

#include <libPOSIXWrapper/POSIXKit/unistd.h>

/// @brief Please use these macros to specify whether your function is thread safe or not.
#define PTHREAD_UNSAFE __THREAD_UNSAFE
#define PTHREAD_SAFE __THREAD_SAFE

PTHREAD_SAFE SInt32 pthread_create(_Output ThreadRef* thread, VoidPtr attr, VoidPtr (*start_routine)(VoidPtr), VoidPtr arg);
PTHREAD_SAFE SInt32 pthread_join(ThreadRef thread, VoidPtr* retval);
PTHREAD_SAFE SInt32 pthread_exit(SInt32 retval);
PTHREAD_SAFE SInt32 pthread_detach(ThreadRef thread);
PTHREAD_SAFE ThreadRef pthread_self(Void);
PTHREAD_SAFE SInt32 pthread_yield(void);

#endif  // LIBPOSIX_POSIXKIT_THREAD_H
