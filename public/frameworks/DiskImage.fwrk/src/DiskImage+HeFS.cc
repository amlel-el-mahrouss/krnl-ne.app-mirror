/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

  FILE: DiskImage+OpenHeFS.cc
  PURPOSE: Disk Imaging framework.

   ======================================== */

#include <DiskImage.fwrk/headers/DiskImage.h>

#include <FSKit/OpenHeFS.h>
#include <FirmwareKit/EPM.h>

/// @brief format OpenHeFS over an EPM disk.
/// @param img disk image structure.
/// @return Status code upon completion.
SInt32 DI::DIFormatFilesystemHeFS(struct DI_DISK_IMAGE& img) {
  if (!img.sector_sz || (img.sector_sz % kDISectorSz != 0)) return kDIFailureStatus;

  if (*img.out_name == 0 || *img.disk_name == 0) return kDIFailureStatus;

  IORef handle = IoOpenFile(img.out_name, nullptr);

  if (!handle) return kDIFailureStatus;

  ::IoCloseFile(handle);

  handle = nullptr;

  return kDISuccessStatus;
}