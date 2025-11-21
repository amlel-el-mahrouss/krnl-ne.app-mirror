/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#include <public/frameworks/KernelTest.fwrk/headers/TestCase.h>

/// ================================================================================
/// @brief Function to run breaking test.
/// ================================================================================
EXTERN_C Kernel::Void KT_TestBreak() {
  KT_RUN_TEST(AlwaysBreak);
}

/// ================================================================================
/// @brief Function to run passing test.
/// ================================================================================
EXTERN_C Kernel::Void KT_TestPass() {
  KT_RUN_TEST(AlwaysPass);
}