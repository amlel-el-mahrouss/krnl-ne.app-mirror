/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <KernelTest.fwrk/headers/SourceLocation.h>

/// ================================================================================
/// @brief Kernel Test Framework main header.
/// @file TestCase.h
/// ================================================================================

#define KT_RUN_TEST(OBJECT) {KTTestCase##OBJECT{}.Run();}

#define KT_MUST_PASS(MSG, LEFT_COND, RIGHT_COND)                                          \
  if (LEFT_COND != RIGHT_COND) {                                                          \
    (Kernel::Void)(Kernel::kout << "[KERNEL-TEST] BREAK: LEFT_COND: " << #LEFT_COND       \
                                << " RIGHT_COND: " << #RIGHT_COND << Kernel::kendl);      \
    (Kernel::Void)(Kernel::kout << "[KERNEL-TEST] BREAK: MSG: " << MSG << Kernel::kendl); \
    MUST_PASS(NO);                                                                        \
  } else {                                                                                \
    (Kernel::Void)(Kernel::kout << "[KERNEL-TEST] PASS: MSG: " << MSG << Kernel::kendl);  \
  }

#define KT_DECL_TEST(NAME, FN)                                       \
  class KTTestCase##NAME final {                                     \
   public:                                                           \
    explicit KTTestCase##NAME() = default;                           \
    ~KTTestCase##NAME()         = default;                           \
    LIBSYS_COPY_DELETE(KTTestCase##NAME);                            \
    Kernel::Void        Run();                                       \
    const Kernel::Char* ToString();                                  \
  };                                                                 \
  inline Kernel::Void KTTestCase##NAME::Run() {                      \
    auto ret = FN() == YES;                                          \
    if (!ret) {                                                      \
      Kernel::kout << "[KERNEL-TEST] TEST FAILED!" << Kernel::kendl; \
      MUST_PASS(ret);                                                \
    }                                                                \
  }                                                                  \
  inline const Kernel::Char* KTTestCase##NAME::ToString() {          \
    return #FN;                                                      \
  }

KT_DECL_TEST(AlwaysBreak, []() -> bool {
  KT_MUST_PASS("AlwaysBreak", YES, NO);
  return NO;
});

KT_DECL_TEST(AlwaysPass, []() -> bool {
  KT_MUST_PASS("AlwaysPass", YES, YES);
  return YES;
});