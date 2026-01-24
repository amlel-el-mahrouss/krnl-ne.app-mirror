// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef KERNELKIT_ZXD_H
#define KERNELKIT_ZXD_H

#include <NeKit/Config.h>

#define kZXDMagicNumber (0x2010AF)
#define kZXDVersion (0x0001)

namespace Kernel {

struct ZxdExec;
struct ZxdStub;

enum struct ZxdFlags : Int32 {
  kZXDFlagsInvalid = 0,
  kZXDFlagsDriver  = 120,
  kZXDFlagsBoot,
  kZXDFlagsKernel,
  kZXDFlagsCount = kZXDFlagsKernel - kZXDFlagsDriver + 1,
};

/// @brief ZXD executable header
/// @details This header is used to identify ZXD executable files.
struct PACKED ZxdExec final {
  UInt32  fMagic;
  UInt32  fVersion;
  UInt32  fFlags;
  UInt32  fHdrSize;
  UInt32  fCRC32;
  UInt32  fAssigneeSignature;
  UInt32  fIssuerSingature;
  UIntPtr fExecOffset;
  SizeT   fExecSize;
  UIntPtr fStubOffset;
  SizeT   fStubSize;
  SizeT   fStubAlign;
  SizeT   fStubCount;
};

/// @brief ZXD stub header
/// @details This header is used to identify ZXD stub files. It contains the size of the stub, the
/// offset of the stub, and the CRC32 checksum of the stub.
struct PACKED ZxdStub final {
  Char   fName[8];
  UInt32 fStubSize;
  UInt32 fStubOffset;
  UInt32 fStubCRC32;
};

inline constexpr auto kDriverName = ".drvr";

/// @note This is ProcessSanitizer specific.
inline constexpr auto kProsanName = ".pros";

using ZxdExecPtr = ZxdExec*;

using ZxdStubPtr = ZxdStub*;

}  // namespace Kernel

#endif
