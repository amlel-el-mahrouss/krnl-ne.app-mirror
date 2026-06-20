// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

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
