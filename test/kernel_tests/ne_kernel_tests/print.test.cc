/// \file kout.test.cc
/// \brief Konsole Out tests.

#include <libSystem/SystemKit/System.h>
#include <public/frameworks/KernelTest/headers/TestCase.h>

/// \note PrintGet tests
KT_DECL_TEST(PrintIsNull, []() -> bool { return PrintGet("/null/") == nullptr; });
KT_DECL_TEST(PrintIsNotNull, []() -> bool { return PrintGet(nullptr) != nullptr; });

/// \note PrintCreate/PrintRelease tests
KT_DECL_TEST(PrintCreateValid, []() -> bool {
  auto handle = PrintCreate();
  if (!handle) return NO;
  PrintRelease(handle);
  return YES;
});

KT_DECL_TEST(PrintReleaseValid, []() -> bool {
  auto handle = PrintCreate();
  if (!handle) return NO;
  return PrintRelease(handle) == 0;
});

KT_DECL_TEST(PrintReleaseNull, []() -> bool { return PrintRelease(nullptr) != 0; });

/// \note PrintOut tests
KT_DECL_TEST(PrintOutValid, []() -> bool {
  SInt32 result = PrintOut(nullptr, "Test output\n");
  return result >= 0;
});

KT_DECL_TEST(PrintOutWithHandle, []() -> bool {
  auto   handle = PrintCreate();
  SInt32 result = PrintOut(handle, "Test with handle\n");
  PrintRelease(handle);
  return result >= 0;
});

KT_DECL_TEST(PrintOutNull, []() -> bool {
  SInt32 result = PrintOut(nullptr, nullptr);
  return result < 0;
});

KT_DECL_TEST(PrintOutFormatted, []() -> bool {
  SInt32 result = PrintOut(nullptr, "Value: %d\n", 42);
  return result >= 0;
});

/// \note PrintIn tests
KT_DECL_TEST(PrintInValid, []() -> bool {
  SInt32 result = PrintIn(nullptr, "Input prompt: ");
  return result >= 0;
});

/// \brief Run 'kout' test.
SInt32 KT_TEST_MAIN() {
  KT_RUN_TEST(PrintIsNull);
  KT_RUN_TEST(PrintIsNotNull);

  KT_RUN_TEST(PrintCreateValid);
  KT_RUN_TEST(PrintReleaseValid);
  KT_RUN_TEST(PrintReleaseNull);

  KT_RUN_TEST(PrintOutValid);
  KT_RUN_TEST(PrintOutWithHandle);
  KT_RUN_TEST(PrintOutNull);
  KT_RUN_TEST(PrintOutFormatted);

  KT_RUN_TEST(PrintInValid);

  return KT_TEST_SUCCESS;
}
