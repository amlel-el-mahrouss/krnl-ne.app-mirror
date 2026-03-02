// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef DRIVERKIT_MACROS_H
#define DRIVERKIT_MACROS_H

#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
#define BOOL bool
#define YES true
#define NO false
#define DDK_EXTERN extern "C"
#define nil nullptr
#undef NULL
#define NULL 0
#define DDK_FINAL final
#else
#define BOOL char
#define YES 1
#define NO 0
#define DDK_EXTERN extern
#define nil ((void*) 0)
#undef NULL
#define NULL ((void*) 0)
#define DDK_FINAL
#endif  // defined(__cplusplus)

#ifndef __DDK__
#undef DDK_EXTERN
#if defined(__cplusplus)
#define DDK_EXTERN extern "C"
#else
#define DDK_EXTERN
#endif
#endif

#define ATTRIBUTE(X) __attribute__((X))

#ifndef __NEOSKRNL__
#error !!! Do not include header in EL0/Ring 3 mode !!!
#endif  // __NEOSKRNL__

#endif
