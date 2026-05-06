// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef NETWORKKIT_MAC_H
#define NETWORKKIT_MAC_H

#include <NeKit/Array.h>
#include <NeKit/Config.h>
#include <NeKit/KString.h>

#define kMACAddrLen (32U)

namespace Kernel {
class MacAddressGetter;

/// \brief This retrieves the MAC address of the device.
/// \note Listens for the current NIC.
class MacAddressGetter final {
 public:
  MacAddressGetter()  = default;
  ~MacAddressGetter() = default;

  NE_COPY_DEFAULT(MacAddressGetter)

 public:
  Array<UInt8, kMACAddrLen>& AsBytes();

 private:
  Array<UInt8, kMACAddrLen> fMacAddress;
};

}  // namespace Kernel

#endif
