// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef STORAGEKIT_PRDT_H
#define STORAGEKIT_PRDT_H

#include <KernelKit/PCI/DMA.h>
#include <KernelKit/PCI/Iterator.h>
#include <NeKit/Ref.h>

#define kPrdtTransferSize (sizeof(Kernel::UShort))

namespace Kernel {

/// @brief Tranfer information about PRD.
enum : Int32 {
  kPRDTTransferInProgress,
  kPRDTTransferIsDone,
  kPRDTTransferCount,
};

/// @brief Physical Region Descriptor Table.
struct PRDT final {
  UInt32 fPhysAddress;
  UInt32 fSectorCount;
  UInt8  fEndBit;
};

using RefPRDT = Ref<PRDT>;

void sk_construct_prdt(Ref<PRDT>& prd);

EXTERN_C Int32 kPRDTTransferStatus;

}  // namespace Kernel

#endif
