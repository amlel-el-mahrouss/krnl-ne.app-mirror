/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <NeKit/Config.h>

#define kZXDMagicNumber (0x2010AF)
#define kZXDVersion (0x0001)

namespace Kernel {
struct ZxdExec;
struct ZxdStub;

enum struct ZxdFlags {
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
inline constexpr auto kProsanName = ".pros";

using ZxdExecPtr = ZxdExec*;
using ZxdStubPtr = ZxdStub*;
}  // namespace Kernel
