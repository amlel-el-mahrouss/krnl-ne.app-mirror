// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

/***
  Dtor and ctors.
*/

#ifndef __INC_NETWORK_DEVICE_H__
#include <NetworkKit/NetworkDevice.h>
#endif  // __INC_NETWORK_DEVICE_H__

namespace Kernel {
inline NetworkDevice::NetworkDevice(
    void (*out)(IDevice<NetworkDeviceCommand>*, NetworkDeviceCommand),
    void (*in)(IDevice<NetworkDeviceCommand>*, NetworkDeviceCommand), void (*on_cleanup)(void))
    : IDevice<NetworkDeviceCommand>(out, in), fCleanup(on_cleanup) {
  kout << "NetworkDevice initialized.\r";

  MUST_PASS(out && in && on_cleanup);
}

inline NetworkDevice::~NetworkDevice() {
  kout << "NetworkDevice cleanup.\r";

  if (fCleanup) fCleanup();
}
}  // namespace Kernel
