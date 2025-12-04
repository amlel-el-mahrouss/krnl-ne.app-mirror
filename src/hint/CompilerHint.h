/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef _NE_COMPILERHINT_H_
#define _NE_COMPILERHINT_H_

#ifdef __NECTI__
#pragma compiler(hint_manifest)
#endif

#define _Input
#define _Output

#define _Optional

#define _StrictCheckInput
#define _StrictCheckOutput

#define _InOut
#define _StrictInOut

#endif  // ifndef _NE_COMPILERHINT_H_
