// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef LIBPOSIX_POSIXKIT_UNISTD_H
#define LIBPOSIX_POSIXKIT_UNISTD_H

#include <libSystem/SystemKit/System.h>

/// @file Unistd.h
/// @brief POSIX Standard Header for NeKernel.

#define _POSIX_SOURCE __POSIX_SOURCE__
#define _XOPEN_SOURCE __XOPEN_SOURCE__

IMPORT_C SInt64 write(SizeT count,
		    SInt32 fd, Void* data, SizeT sz);

IMPORT_C SInt64 read(SizeT count,
		    SInt32 fd, Void* data, SizeT sz);

#endif  // LIBPOSIX_POSIXKIT_UNISTD_H
