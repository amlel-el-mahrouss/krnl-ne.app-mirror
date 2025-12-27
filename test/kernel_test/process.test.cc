/// \file process.test.cc
/// \brief Process Out tests.
/// \author Amlal El Mahrouss (amlal at nekernel dot org)

#include <libSystem/SystemKit/System.h>
#include <public/frameworks/KernelTest.fwrk/headers/TestCase.h>

/// \note Declare tests
KT_DECL_TEST(ProcessHasFailed, []() -> bool {
  /// \todo we return -1 here, should we document that or classify as common knowledge?
  return RtlSpawnProcess("/", 0, nullptr, nullptr, 0) == -1;
});

KT_DECL_TEST(ProcessHasSucceeded, []() -> bool {
  /// \note Any process greater than zero, exists within a specific team domain (real-time, high, or
  /// low domains).
  return RtlSpawnProcess("/system/list", 0, nullptr, nullptr, 0) > 0;
});

/// \brief Run 'process' test.
SInt32 KT_TEST_MAIN() {
  KT_RUN_TEST(ProcessHasFailed);
  KT_RUN_TEST(ProcessHasSucceeded);

  return KT_TEST_SUCCESS;
}
