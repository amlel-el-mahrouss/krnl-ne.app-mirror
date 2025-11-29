/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#include <BootKit/BootKit.h>
#include <BootKit/Platform.h>
#include <BootKit/Protocol.h>

#ifdef __BOOTZ_STANDALONE__

using namespace Boot;

EXTERN_C void rt_halt() {
  while (Yes);
}

EXTERN_C void rt_cli() {}

EXTERN_C void rt_sti() {}

EXTERN_C void rt_cld() {}

EXTERN_C void rt_std() {}

#endif  // __BOOTZ_STANDALONE__
