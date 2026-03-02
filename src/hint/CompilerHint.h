// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef NE_COMPILERHINT_H
#define NE_COMPILERHINT_H

#ifdef __NECTAR__
#pragma compiler(hint_manifest)
#endif

#define _Input
#define _Output

#define _Optional

#define _StrictCheckInput
#define _StrictCheckOutput

#define _InOut
#define _StrictInOut

#define _OptionalIn
#define _OptionalOut

#endif  // ifndef NE_COMPILERHINT_H
