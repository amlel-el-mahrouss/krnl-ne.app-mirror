// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef KERNELKIT_XCOFF_H
#define KERNELKIT_XCOFF_H

#include <NeKit/Config.h>

#define kXCOFF64Magic (0x01F7)
#define kXCOFF64ForkNameLen (256U)

#define kXCOFFRelFlg (0x0001)
#define kXCOFFExecutable (0x0002)
#define kXCOFFLnno (0x0004)
#define kXCOFFLSyms (0x0008)

namespace Kernel {

struct XCOFF_FILE_HEADER;
struct XCOFF_FORK_HEADER;

/// @brief XCoff file header, meant for POWER apps.
typedef struct XCOFF_FILE_HEADER {
  Kernel::UInt16  fMagic;
  Kernel::UInt16  fTarget;
  Kernel::UInt16  fNumSecs;
  Kernel::UInt32  fTimeDat;
  Kernel::UIntPtr fSymPtr;
  Kernel::UInt32  fNumSyms;
  Kernel::UInt16  fOptHdr;  // ?: Number of bytes in optional header
} XCOFF_FILE_HEADER, XCOFF_FILE_HEADER32, XCOFF_FILE_HEADER64;

/// @brief This the executable's manifest fork, designed for NeFS.
/// @param fPropertiesXMLFork The XML fork of the executable.
/// @param fDynamicLoaderFork The DYLD fork metadata.
/// @param fCodeSignFork Executable's certificate contained in a fork.
typedef struct XCOFF_FORK_HEADER {
  Kernel::Char fPropertiesXMLFork[kXCOFF64ForkNameLen];
  Kernel::Char fDynamicLoaderFork[kXCOFF64ForkNameLen];
  Kernel::Char fCodeSignFork[kXCOFF64ForkNameLen];
} XCOFF_FORK_HEADER;

}  // namespace Kernel

#endif  // ifndef KERNELKIT_XCOFF_H
