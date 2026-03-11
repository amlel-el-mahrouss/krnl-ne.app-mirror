// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel
#include <KernelKit/DebugOutput.h>
#include <NeKit/KString.h>
#include <StorageKit/PRDT.h>

namespace Kernel {
/***********************************************************************************/
/// @brief constructs a new PRD.
/// @param prd PRD reference.
/// @note This doesnt construct a valid, please fill it by yourself.
/***********************************************************************************/
void construct_prdt(Ref<PRDT>& prd) {
  prd.Leak().fPhysAddress = 0x0;
  prd.Leak().fSectorCount = 0x0;
  prd.Leak().fEndBit      = 0x0;
}
}  // namespace Kernel
