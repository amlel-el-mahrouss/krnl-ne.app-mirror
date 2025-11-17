/* -------------------------------------------

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

------------------------------------------- */

#include <NetworkKit/MAC.h>

namespace Kernel {
Array<UInt8, kMACAddrLen>& MacAddressGetter::AsBytes() {
  return this->fMacAddress;
}
}  // namespace Kernel
