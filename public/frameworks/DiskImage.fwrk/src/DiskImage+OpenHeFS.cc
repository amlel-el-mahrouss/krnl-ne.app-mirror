// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

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