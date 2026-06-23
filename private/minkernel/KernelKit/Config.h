// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef KERNELKIT_CONFIG_H
#define KERNELKIT_CONFIG_H

#include <NeKit/Config.h>

#define KERNELKIT_VERSION "0.0.3"
#define KERNELKIT_VERSION_BCD 0x0003

namespace Ne::Kernel {
class UserProcessScheduler;
class IDylibObject;
class UserProcess;
class KernelTask;
}  // namespace Ne::Kernel

#endif
