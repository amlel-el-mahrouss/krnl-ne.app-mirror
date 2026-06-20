// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef BOOTKIT_PLATFORM_H
#define BOOTKIT_PLATFORM_H

/**
  @file Platform.h
  @brief Platform specific code.
*/

#ifdef __x86_64__

#ifdef __cplusplus
#ifndef EXTERN_C
#define EXTERN_C extern "C"
#endif
#else
#ifndef EXTERN_C
#define EXTERN_C extern
#endif
#endif  // __cplusplus

EXTERN_C void rt_halt();
EXTERN_C void rt_cli();
EXTERN_C void rt_sti();
EXTERN_C void rt_cld();
EXTERN_C void rt_std();

#endif /* ifdef __x86_64__ */

#endif
