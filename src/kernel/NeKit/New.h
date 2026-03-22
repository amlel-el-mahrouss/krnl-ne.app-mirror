// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef NEKIT_NEW_H
#define NEKIT_NEW_H

#include <KernelKit/HeapMgr.h>

/// @note compatible with tk too.
typedef __SIZE_TYPE__ size_t;

void* operator new(size_t);
void* operator new[](size_t);

void operator delete(void*) noexcept;
void operator delete(void*, unsigned long) noexcept;
void operator delete[](void*) noexcept;

#endif
