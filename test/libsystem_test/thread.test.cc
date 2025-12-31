/// \file thread.test.cc
/// \brief Threading API tests.
/// \author Amlal El Mahrouss (amlal at nekernel dot org)

#include <libSystem/SystemKit/System.h>
#include <public/frameworks/KernelTest.fwrk/headers/TestCase.h>

/// \note Thread procedure for testing
static SInt32 test_thread_proc(SInt32 argc, Char** argv) {
  return 0;
}

static SInt32 test_thread_proc_with_arg(SInt32 argc, Char** argv) {
  if (argc > 0 && argv) {
    return 1;
  }
  return 0;
}

/// \note Thread creation tests

KT_DECL_TEST(ThrCreateThreadValid, []() -> bool {
  auto thread = ThrCreateThread("test_thread", test_thread_proc, 0, 0);
  if (!thread) return NO;
  ThrJoinThread(thread);
  return YES;
});

KT_DECL_TEST(ThrCreateThreadNull, []() -> bool {
  auto thread = ThrCreateThread(nullptr, nullptr, 0, 0);
  return thread == nullptr;
});

KT_DECL_TEST(ThrCreateThreadWithName, []() -> bool {
  auto thread = ThrCreateThread("named_thread", test_thread_proc, 0, 0);
  if (!thread) return NO;
  ThrJoinThread(thread);
  return YES;
});

/// \note Thread lifecycle tests

KT_DECL_TEST(ThrJoinThreadValid, []() -> bool {
  auto thread = ThrCreateThread("join_test", test_thread_proc, 0, 0);
  if (!thread) return NO;

  SInt32 result = ThrJoinThread(thread);
  return result == 0;
});

KT_DECL_TEST(ThrDetachThreadValid, []() -> bool {
  auto thread = ThrCreateThread("detach_test", test_thread_proc, 0, 0);
  if (!thread) return NO;

  SInt32 result = ThrDetachThread(thread);
  return result == 0;
});

KT_DECL_TEST(ThrYieldThreadValid, []() -> bool {
  auto thread = ThrCreateThread("yield_test", test_thread_proc, 0, 0);
  if (!thread) return NO;

  SInt32 result = ThrYieldThread(thread);
  ThrJoinThread(thread);

  return result == 0;
});

/// \note Thread exit tests

KT_DECL_TEST(ThrExitThreadValid, []() -> bool {
  auto thread = ThrCreateThread("exit_test", test_thread_proc, 0, 0);
  if (!thread) return NO;

  SInt32 result = ThrExitThread(thread, 0);
  return result == 0;
});

KT_DECL_TEST(ThrExitThreadWithCode, []() -> bool {
  auto thread = ThrCreateThread("exit_code_test", test_thread_proc, 0, 0);
  if (!thread) return NO;

  SInt32 result = ThrExitThread(thread, 42);
  return result == 0;
});

/// \note Thread argument tests

KT_DECL_TEST(ThrCreateThreadWithArgs, []() -> bool {
  auto thread = ThrCreateThread("args_test", test_thread_proc_with_arg, 1, 0);
  if (!thread) return NO;
  ThrJoinThread(thread);
  return YES;
});

KT_DECL_TEST(ThrCreateThreadMultiple, []() -> bool {
  auto thread1 = ThrCreateThread("multi_test_1", test_thread_proc, 0, 0);
  auto thread2 = ThrCreateThread("multi_test_2", test_thread_proc, 0, 0);

  if (!thread1 || !thread2) return NO;

  ThrJoinThread(thread1);
  ThrJoinThread(thread2);

  return YES;
});

/// \note Current thread tests

KT_DECL_TEST(ThrExitCurrentThreadValid, []() -> bool {
  // Note: Can't directly test this as it would exit the test thread
  // This is a placeholder for integration testing
  return YES;
});

KT_DECL_TEST(ThrExitMainThreadValid, []() -> bool {
  // Note: Can't directly test this as it would exit the main thread
  // This is a placeholder for integration testing
  return YES;
});

/// \brief Run threading tests.
SInt32 KT_TEST_MAIN() {
  KT_RUN_TEST(ThrCreateThreadValid);
  KT_RUN_TEST(ThrCreateThreadNull);
  KT_RUN_TEST(ThrCreateThreadWithName);

  KT_RUN_TEST(ThrJoinThreadValid);
  KT_RUN_TEST(ThrDetachThreadValid);
  KT_RUN_TEST(ThrYieldThreadValid);

  KT_RUN_TEST(ThrExitThreadValid);
  KT_RUN_TEST(ThrExitThreadWithCode);

  KT_RUN_TEST(ThrCreateThreadWithArgs);
  KT_RUN_TEST(ThrCreateThreadMultiple);

  KT_RUN_TEST(ThrExitCurrentThreadValid);
  KT_RUN_TEST(ThrExitMainThreadValid);

  return KT_TEST_SUCCESS;
}
