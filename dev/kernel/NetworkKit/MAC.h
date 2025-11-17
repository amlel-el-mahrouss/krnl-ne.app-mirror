/* -------------------------------------------

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

------------------------------------------- */

#pragma once

#include <NeKit/Array.h>
#include <NeKit/Defines.h>
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
