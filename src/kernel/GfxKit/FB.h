/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <KernelKit/DeviceMgr.h>

namespace Kernel {
class FBDeviceInterface;
struct FBDevicePacket;

typedef UInt32 FBCoord2x2;
typedef UInt32 FBDim2x2;
typedef UInt32 FBColorProfile;
typedef UInt32 FBFlags;

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
  explicit FBDeviceInterface(void (*out)(IDevice* self, FBDevicePacket* out),
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
