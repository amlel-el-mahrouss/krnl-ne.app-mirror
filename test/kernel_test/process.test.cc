/// \file process.test.cc
/// \brief Process Out tests.

#include <libSystem/SystemKit/System.h>
#include <public/frameworks/KernelTest.fwrk/headers/TestCase.h>

/// \note Declare tests
KT_DECL_TEST(ProcessHasFailed, []() -> bool {
  /// \todo we return -1 here, should we document that or classify as common knowledge?
  return RtlSpawnProcess("/system/ls", 0, nullptr, nullptr, 0) == -1;
});

/// \brief Run 'process' test.
SInt32 KT_TEST_MAIN() {
  KT_RUN_TEST(ProcessHasFailed);

  return KT_TEST_SUCCESS;
}
