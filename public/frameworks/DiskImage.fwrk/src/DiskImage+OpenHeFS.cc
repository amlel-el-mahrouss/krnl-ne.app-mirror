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
SInt32 DI::DIFormatFilesystemOpenHeFS(struct DI_DISK_IMAGE& img) {
  NE_UNUSED(img);

  PrintOut(nullptr, "%s",
           "DIFormatFilesystemOpenHeFS is not formattable on DI, work may be done in the future to "
           "address that.");

  return kDIFailureStatus;
}