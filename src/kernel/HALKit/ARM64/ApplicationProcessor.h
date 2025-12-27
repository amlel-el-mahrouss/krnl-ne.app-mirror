// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef HALKIT_APPLICATIONPROCESSOR_H
#define HALKIT_APPLICATIONPROCESSOR_H

#include <HALKit/ARM64/Processor.h>
#include <NeKit/Config.h>

/************************************************** */
/*     INITIALIZE THE GIC ON THE CURRENT CORE.      */
/*     WITH AN EXECUTION LEVEL IN MIND.             */
/************************************************** */

namespace Kernel {
Void mp_init_cores(Void);
}

#endif
