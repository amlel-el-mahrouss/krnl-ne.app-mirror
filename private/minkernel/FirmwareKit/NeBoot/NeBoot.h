// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef FIRMWAREKIT_NEBOOT_H
#define FIRMWAREKIT_NEBOOT_H

#include <NeKit/Config.h>

namespace Firmware::Detail::NeBoot {
using namespace Kernel;

struct NEBOOT_LINEAR_EXEC;

/// @brief Linear Executable Header
/// @author Amlal El Mahrouss
struct ATTRIBUTE(aligned(4)) NEBOOT_LINEAR_EXEC {
  const Char   fMagic[2];      // magic number
  const Char   fName[10];      // operating system name
  const UInt32 fRevision;      // firmware revision
  const UInt32 fStartAddress;  // start address (master/slave(s) thread)

#ifdef NE_IS_EXTENDED_COREBOOT
  UIntPtr fMasterStructure;         // master structure for MP/PM and device tree and such. (ARM)
  UIntPtr fMasterStructureVersion;  // master structure version.
#endif

#ifdef NE_IS_MBCI_COREBOOT
  UIntPtr fMBCIStructure;         // MBCI structure for MBCI (ARM)
  UIntPtr fMBCIStructureVersion;  // MBCI structure version.
#endif
};
}  // namespace Firmware::Detail::NeBoot

#endif
