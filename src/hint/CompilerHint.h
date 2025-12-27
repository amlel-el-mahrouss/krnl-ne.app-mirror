// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

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
