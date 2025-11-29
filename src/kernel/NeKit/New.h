
/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <KernelKit/HeapMgr.h>

/// @note compatible with tk too.
typedef __SIZE_TYPE__ size_t;

void* operator new(size_t);
void* operator new[](size_t);

void operator delete(void*);
void operator delete(void*, unsigned long);
void operator delete[](void*);
