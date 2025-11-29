/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <HALKit/ARM64/Processor.h>
#include <NeKit/Defines.h>

/************************************************** */
/*     INITIALIZE THE GIC ON THE CURRENT CORE.      */
/*     WITH AN EXECUTION LEVEL IN MIND.             */
/************************************************** */

namespace Kernel {
Void mp_init_cores(Void);
}