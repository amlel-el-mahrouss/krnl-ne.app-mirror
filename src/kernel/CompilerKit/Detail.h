/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef COMPILERKIT_DETAIL_H
#define COMPILERKIT_DETAIL_H

#ifdef __NEOSKRNL__
#include <NeKit/Config.h>
#endif  // ifdef __NEOSKRNL__

#define NE_COPY_DELETE(KLASS)              \
  KLASS& operator=(const KLASS&) = delete; \
  KLASS(const KLASS&)            = delete;

#define NE_COPY_DEFAULT(KLASS)              \
  KLASS& operator=(const KLASS&) = default; \
  KLASS(const KLASS&)            = default;

#define NE_MOVE_DELETE(KLASS)         \
  KLASS& operator=(KLASS&&) = delete; \
  KLASS(KLASS&&)            = delete;

#define NE_MOVE_DEFAULT(KLASS)         \
  KLASS& operator=(KLASS&&) = default; \
  KLASS(KLASS&&)            = default;

#endif
