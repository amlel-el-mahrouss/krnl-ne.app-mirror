/// \file kout.test.cc
/// \brief Konsole Out tests.

#include <libSystem/SystemKit/System.h>
#include <public/frameworks/KernelTest.fwrk/headers/TestCase.h>

/// \note Declare tests
KT_DECL_TEST(KOutIsNull, []() -> bool { return PrintGet("/null/") == nullptr; });
KT_DECL_TEST(KOutIsNotNull, []() -> bool { return PrintGet(nullptr) != nullptr; });

/// \brief Run 'kout' test.
SInt32 KT_TEST_MAIN() {
  KT_RUN_TEST(KOutIsNull);
  KT_RUN_TEST(KOutIsNotNull);

  return KT_TEST_SUCCESS;
}