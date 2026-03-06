/// \file process.test.cc
/// \brief Process management tests.
/// \author Amlal El Mahrouss (amlal at nekernel dot org)

#include <KernelTest.fwrk/headers/TestCase.h>
#include <libSystem/SystemKit/System.h>

/// \note RtlSpawnProcess tests
KT_DECL_TEST(ProcessHasFailed, []() -> bool {
  /// \todo we return -1 here, should we document that or classify as common knowledge?
  return RtlSpawnProcess("/", 0, nullptr, nullptr, 0) == -1;
});

KT_DECL_TEST(ProcessHasSucceeded, []() -> bool {
  /// \note Any process greater than zero, exists within a specific team domain (real-time, high, or
  /// low domains).
  return RtlSpawnProcess("/system/list", 0, nullptr, nullptr, 0) > 0;
});

KT_DECL_TEST(ProcessSpawnNullPath,
             []() -> bool { return RtlSpawnProcess(nullptr, 0, nullptr, nullptr, 0) == -1; });

KT_DECL_TEST(ProcessSpawnInvalidPath, []() -> bool {
  return RtlSpawnProcess("/invalid/nonexistent", 0, nullptr, nullptr, 0) == -1;
});

/// \note RtlSpawnIB tests
KT_DECL_TEST(ProcessSpawnIBValid, []() -> bool {
  UIntPtr pid = RtlSpawnProcess("/system/list", 0, nullptr, nullptr, 0);
  if (pid <= 0) return NO;

  UInt32 result = RtlSpawnIB(pid);
  return result == 0;
});

KT_DECL_TEST(ProcessSpawnIBInvalid, []() -> bool {
  UInt32 result = RtlSpawnIB(0);
  return result > 0;
});

/// \note RtlExitProcess tests
KT_DECL_TEST(ProcessExitValid, []() -> bool {
  UIntPtr pid = RtlSpawnProcess("/system/list", 0, nullptr, nullptr, 0);
  if (pid <= 0) return NO;

  Bool result = RtlExitProcess(pid, 0);
  return result == YES;
});

KT_DECL_TEST(ProcessExitWithCode, []() -> bool {
  UIntPtr pid = RtlSpawnProcess("/system/list", 0, nullptr, nullptr, 0);
  if (pid <= 0) return NO;

  Bool result = RtlExitProcess(pid, 42);
  return result == YES;
});

KT_DECL_TEST(ProcessExitInvalid, []() -> bool {
  Bool result = RtlExitProcess(0, 0);
  return result == NO;
});

/// \brief Run 'process' test.
IMPORT_C SInt32 KT_TEST_MAIN() {
  KT_RUN_TEST(ProcessHasFailed);
  KT_RUN_TEST(ProcessHasSucceeded);
  KT_RUN_TEST(ProcessSpawnNullPath);
  KT_RUN_TEST(ProcessSpawnInvalidPath);

  KT_RUN_TEST(ProcessSpawnIBValid);
  KT_RUN_TEST(ProcessSpawnIBInvalid);

  KT_RUN_TEST(ProcessExitValid);
  KT_RUN_TEST(ProcessExitWithCode);
  KT_RUN_TEST(ProcessExitInvalid);

  return KT_TEST_SUCCESS;
}
