// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel
#ifndef HEADERS_TESTCASE_H
#define HEADERS_TESTCASE_H

#include <KernelTest.fwrk/headers/SourceLocation.h>

/// ================================================================================
/// @brief Kernel Test Framework main header.
/// @file TestCase.h
/// ================================================================================

#define KT_RUN_TEST(OBJECT)   \
  {                           \
    KTTestCase##OBJECT obj{}; \
    obj.Run();                \
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
      KT_MUST_PASS(ret, ret, true);                    \
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

#endif
