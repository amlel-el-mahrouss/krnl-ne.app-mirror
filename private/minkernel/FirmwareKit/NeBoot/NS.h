// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef FIRMWAREKIT_NS_H
#define FIRMWAREKIT_NS_H

/// @brief internal macro
#ifdef NE_WRAP_NEBOOT
namespace NeBoot {
#endif

#include <FirmwareKit/NeBoot/BootNet.h>
#include <FirmwareKit/NeBoot/NeBoot.h>

#ifdef NE_WRAP_NEBOOT
}
#endif

#endif
