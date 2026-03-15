// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <libPThread/PThreadKit/Thread.h>

namespace POSIXKit::Detail {

  /// @brief Max path structure.
  constexpr auto kMaxPathLen = 255;
  static constexpr auto kCanaryValue = 0xf0f0488f;

  /// @brief Thread Information Structure.
  struct ThreadFrameParams final {
    SInt64  fCanary;
    VoidPtr fStackPtr;
    VoidPtr fCodePtr;
    SizeT   fCodeSz;
    SizeT   fStackSz;
    SInt64  fThrdID;
    SInt64  fUsrID, fGrpID;
    SInt64*  fFD{};
    SizeT    fFDCnt;
    Char    fCWD[kMaxPathLen];
    Char    fRoot[kMaxPathLen];
    ThreadRef fRef;
  };

} // namespace POSIX::Detail



