// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/kernel

#include <KernelKit/FileMgr.h>
#include <KernelKit/HeapMgr.h>
#include <KernelKit/KPC.h>
#include <KernelKit/ThreadLocalStorage.h>
#include <KernelKit/UserMgr+User.h>
#include <NeKit/KString.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/Utils.h>

/// @file UserMgr+User.cpp
/// @brief Multi-user support.

namespace Kernel {
namespace Detail {
  struct UserPermissionControl;
  struct UserPermissionElevator;
}  // namespace Detail
}  // namespace Kernel
