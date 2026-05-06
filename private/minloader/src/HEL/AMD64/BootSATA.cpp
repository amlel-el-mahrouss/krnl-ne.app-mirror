// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#include <BootKit/HW/SATA.h>
#include <BootKit/Platform.h>
#include <BootKit/Protocol.h>

#include <BootKit/BootKit.h>
#include <FirmwareKit/EFI.h>

#if defined(__AHCI__) && defined(__SYSCHK__)

using namespace Boot;

/***
 *
 *
 * @brief SATA Device class.
 *
 *
 */

/**
 * @brief ATA Device constructor.
 * @param void none.
 */
BootDeviceSATA::BootDeviceSATA() {
  UInt16 pi = 0u;
  drv_std_init(pi);
}

/**
  @brief Read Buf from disk
  @param Sz Sector size
  @param Buf buffer
*/
BootDeviceSATA& BootDeviceSATA::Read(CharacterTypeASCII* Buf, SizeT SectorSz) {
  drv_std_read(mTrait.mBase / SectorSz, Buf, SectorSz, mTrait.mSize);

  return *this;
}

/**
  @brief Write Buf into disk
  @param Sz Sector size
  @param Buf buffer
*/
BootDeviceSATA& BootDeviceSATA::Write(CharacterTypeASCII* Buf, SizeT SectorSz) {
  drv_std_write(mTrait.mBase / SectorSz, Buf, SectorSz, mTrait.mSize);

  return *this;
}

/**
 * @brief ATA trait getter.
 * @return BootDeviceSATA::ATATrait& the drive config.
 */
BootDeviceSATA::SATATrait& BootDeviceSATA::Leak() {
  return mTrait;
}

#endif