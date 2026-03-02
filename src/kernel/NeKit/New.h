// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef NEKIT_NEW_H
#define NEKIT_NEW_H

#include <KernelKit/HeapMgr.h>

/// @note compatible with tk too.
typedef __SIZE_TYPE__ size_t;

void* operator new(size_t);
void* operator new[](size_t);

void operator delete(void*);
void operator delete(void*, unsigned long);
void operator delete[](void*);

#endif
