// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel
#ifndef FIRMWAREKIT_NS_H
#define FIRMWAREKIT_NS_H

#include <NeKit/Config.h>

namespace Firmware::Detail::EFI {
using namespace Kernel;

EXTERN_C {
#include <FirmwareKit/EFI/EFI.h>
}

}  // namespace Firmware::Detail::EFI

#endif
