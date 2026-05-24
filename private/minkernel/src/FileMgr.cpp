// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#include <KernelKit/FileMgr.h>
#include <NeKit/Utils.h>

/***********************************************************************************/
//! @file FileMgr.cpp
//! @brief File System Manager API.
//! @author Amlal El Mahrouss (amlal@nekernel.org)
/***********************************************************************************/

namespace Kernel {
STATIC IFilesystemMgr* kMountedFilesystem = nullptr;

/// @brief FilesystemMgr getter.
/// @return The mounted filesystem.
_Output IFilesystemMgr* IFilesystemMgr::GetMounted() {
  MUST_PASS(kMountedFilesystem);
  return kMountedFilesystem;
}

/// @brief Unmount filesystem.
/// @return The unmounted filesystem.
_Output IFilesystemMgr* IFilesystemMgr::Unmount() {
  if (kMountedFilesystem) {
    auto mount = kMountedFilesystem;

    kMountedFilesystem = nullptr;
    return mount;
  }

  return nullptr;
}

/// @brief Mount filesystem.
/// @param mount_ptr The filesystem to mount.
/// @return if it succeeded true, otherwise false.
_Output Bool IFilesystemMgr::Mount(_Input IFilesystemMgr* mount_ptr) {
  if (mount_ptr != nullptr) {
    kMountedFilesystem = mount_ptr;
    return Yes;
  }

  return No;
}
}  // namespace Kernel
