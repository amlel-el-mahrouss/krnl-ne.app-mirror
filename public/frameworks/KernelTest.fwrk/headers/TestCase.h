/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <KernelTest.fwrk/headers/SourceLocation.h>

/// ================================================================================
/// @brief Kernel Test Framework main header.
/// @file TestCase.h
/// ================================================================================

#define KT_RUN_TEST(OBJECT)     \
  {                             \
    KTTestCase##OBJECT{}.Run(); \
  }

#define KT_MUST_PASS(MSG, LEFT_COND, RIGHT_COND) \
  if (LEFT_COND != RIGHT_COND) {                 \
    MUST_PASS(NO);                               \
  }

#define KT_DECL_TEST(NAME, FN)                         \
  class KTTestCase##NAME final {                       \
   public:                                             \
    KTTestCase##NAME()  = default;                     \
    ~KTTestCase##NAME() = default;                     \
    LIBSYS_COPY_DELETE(KTTestCase##NAME);              \
    Void        Run();                                 \
    const Char* ToString();                            \
  };                                                   \
  inline Void KTTestCase##NAME::Run() {                \
    auto ret = FN() == YES;                            \
    if (!ret) {                                        \
      PrintOut(nullptr, "[KERNEL-TEST] TEST FAILED!"); \
      MUST_PASS(ret);                                  \
    }                                                  \
  }                                                    \
  inline const Char* KTTestCase##NAME::ToString() {    \
    return #FN;                                        \
  }

KT_DECL_TEST(AlwaysBreak, []() -> bool {
  KT_MUST_PASS("AlwaysBreak", YES, NO);
  return NO;
});

KT_DECL_TEST(AlwaysPass, []() -> bool {
  KT_MUST_PASS("AlwaysPass", YES, YES);
  return YES;
});