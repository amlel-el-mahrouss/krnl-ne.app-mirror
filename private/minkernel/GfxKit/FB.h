// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef GFXKIT_FB_H
#define GFXKIT_FB_H

#include <KernelKit/DeviceMgr.h>

namespace Kernel {

class FBDeviceInterface;
struct FBDevicePacket;

using FBCoord2x2     = UInt32;
using FBDim2x2       = UInt32;
using FBColorProfile = UInt32;
using FBFlags        = UInt32;

/// @brief Framebuffer device interface packet.
/// @details This structure is used to send and receive data from the framebuffer device.
/// @note The structure is packed to ensure that the data is aligned correctly for the device.
struct PACKED FBDevicePacket final {
  FBCoord2x2     fX;
  FBCoord2x2     fY;
  FBDim2x2       fWidth;
  FBDim2x2       fHeight;
  FBColorProfile fColor;
  FBFlags        fFlags;
};

/// @brief Framebuffer device interface.
/// @details This class is used to send and receive data from the framebuffer device.
/// @note The class is derived from the IDevice class.
class FBDeviceInterface NE_DEVICE<FBDevicePacket*> {
 public:
  FBDeviceInterface(void (*out)(IDevice* self, FBDevicePacket* out),
                    void (*in)(IDevice* self, FBDevicePacket* in));

  virtual ~FBDeviceInterface() override;

 public:
  FBDeviceInterface& operator=(const FBDeviceInterface&) = default;
  FBDeviceInterface(const FBDeviceInterface&)            = default;

  const Char* Name() const override;

 public:
  FBDeviceInterface& operator<<(FBDevicePacket* dat) override;
  FBDeviceInterface& operator>>(FBDevicePacket* dat) override;
};

}  // namespace Kernel

#endif
