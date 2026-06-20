// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef GFXKIT_FB_H
#define GFXKIT_FB_H

#include <KernelKit/DeviceMgr.h>

namespace Kernel {

class GPUDeviceInterface;

using GPUCoord2x2     = UInt32;
using GPUDim2x2       = UInt32;
using GPUColorProfile = UInt32;
using GPUFlags        = UInt32;

/// @brief Framebuffer device interface.
/// @details This class is used to send and receive data from the framebuffer device.
/// @note The class is derived from the IDevice class.
class GPUDeviceInterface NE_DEVICE<VoidPtr> {
 public:
  GPUDeviceInterface(void (*out)(IDevice* self, GPUDeviceInterface* out),
                     void (*in)(IDevice* self, GPUDeviceInterface* in));

  virtual ~GPUDeviceInterface() override;

 public:
  GPUDeviceInterface& operator=(const GPUDeviceInterface&) = default;
  GPUDeviceInterface(const GPUDeviceInterface&)            = default;
  const Char* Name() const override;

 public:
  GPUDeviceInterface& operator<<(VoidPtr dat) override;
  GPUDeviceInterface& operator>>(VoidPtr dat) override;
};

}  // namespace Kernel

#endif
