/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <KernelTest.fwrk/headers/Foundation.h>

class KTSourceLocation;

/// ================================================================================
/// @brief SourceLocation class for Kernel Test Framework.
/// ================================================================================
class KTSourceLocation final CF_OBJECT {
 public:
  KTSourceLocation()  = delete;
  ~KTSourceLocation() override = default;

  LIBSYS_COPY_DELETE(KTSourceLocation);

 public:
  KTSourceLocation(const Char*, const SInt32 = 0UL);

  CF::CFString& File() { return mFile; }
  SInt32        Line() { return mLine; }

  CF::CFString operator()();

 private:
  CF::CFString mFile{4096};
  SInt32       mLine{0U};
};