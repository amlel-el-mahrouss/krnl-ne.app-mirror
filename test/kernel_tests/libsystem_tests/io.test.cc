/// \file io.test.cc
/// \brief File I/O API tests.
/// \author Amlal El Mahrouss (amlal at nekernel dot org)

#include <libSystem/SystemKit/System.h>
#include <public/frameworks/KernelTest.fwrk/headers/TestCase.h>

/// \note File open/close tests

KT_DECL_TEST(IoOpenFileInvalid, []() -> bool {
  auto file = IoOpenFile(nullptr, nullptr);
  return file == nullptr;
});

KT_DECL_TEST(IoOpenFileValidPath, []() -> bool {
  auto file = IoOpenFile("/test.txt", nullptr);
  if (!file) return NO;
  IoCloseFile(file);
  return YES;
});

KT_DECL_TEST(IoCloseFileValid, []() -> bool {
  auto file = IoOpenFile("/test.txt", nullptr);
  if (!file) return NO;
  IoCloseFile(file);
  return YES;
});

/// \note File positioning tests

KT_DECL_TEST(IoSeekFileValid, []() -> bool {
  auto file = IoOpenFile("/test.txt", nullptr);
  if (!file) return NO;

  UInt64 pos = IoSeekFile(file, 100);
  IoCloseFile(file);

  return pos != ~0UL;
});

KT_DECL_TEST(IoTellFileValid, []() -> bool {
  auto file = IoOpenFile("/test.txt", nullptr);
  if (!file) return NO;

  IoSeekFile(file, 50);
  UInt64 pos = IoTellFile(file);
  IoCloseFile(file);

  return pos == 50;
});

KT_DECL_TEST(IoRewindFileValid, []() -> bool {
  auto file = IoOpenFile("/test.txt", nullptr);
  if (!file) return NO;

  IoSeekFile(file, 100);
  IoRewindFile(file);
  UInt64 pos = IoTellFile(file);
  IoCloseFile(file);

  return pos == 0;
});

/// \note File read/write tests

KT_DECL_TEST(IoWriteFileValid, []() -> bool {
  auto file = IoOpenFile("/test_write.txt", nullptr);
  if (!file) return NO;

  const char data[]  = "Test data";
  UInt32     written = IoWriteFile(file, (VoidPtr) data, sizeof(data));
  IoCloseFile(file);

  return written == sizeof(data);
});

KT_DECL_TEST(IoReadFileValid, []() -> bool {
  auto file = IoOpenFile("/test_read.txt", nullptr);
  if (!file) return NO;

  char    buffer[64];
  VoidPtr buf_ptr = buffer;
  UInt32  read    = IoReadFile(file, &buf_ptr, sizeof(buffer));
  IoCloseFile(file);

  return read > 0;
});

KT_DECL_TEST(IoWriteFileNull, []() -> bool {
  auto file = IoOpenFile("/test.txt", nullptr);
  if (!file) return NO;

  UInt32 written = IoWriteFile(file, nullptr, 10);
  IoCloseFile(file);

  return written == 0;
});

/// \note File metadata tests

KT_DECL_TEST(IoMimeFileValid, []() -> bool {
  auto file = IoOpenFile("/test.txt", nullptr);
  if (!file) return NO;

  const Char* mime = IoMimeFile(file);
  IoCloseFile(file);

  return mime != nullptr;
});

KT_DECL_TEST(IoDimFileValid, []() -> bool {
  auto file = IoOpenFile("/test_dir", nullptr);
  if (!file) return NO;

  const Char* dim = IoDimFile(file);
  IoCloseFile(file);

  return dim != nullptr;
});

/// \note File control tests

KT_DECL_TEST(IoCtrlFileValid, []() -> bool {
  auto file = IoOpenFile("/test.txt", nullptr);
  if (!file) return NO;

  char   in_data[16]  = {0};
  char   out_data[16] = {0};
  SInt32 result       = IoCtrlFile(file, 1, in_data, out_data);
  IoCloseFile(file);

  return result != 0;
});

/// \brief Run file I/O tests.
SInt32 KT_TEST_MAIN() {
  KT_RUN_TEST(IoOpenFileInvalid);
  KT_RUN_TEST(IoOpenFileValidPath);
  KT_RUN_TEST(IoCloseFileValid);

  KT_RUN_TEST(IoSeekFileValid);
  KT_RUN_TEST(IoTellFileValid);
  KT_RUN_TEST(IoRewindFileValid);

  KT_RUN_TEST(IoWriteFileValid);
  KT_RUN_TEST(IoReadFileValid);
  KT_RUN_TEST(IoWriteFileNull);

  KT_RUN_TEST(IoMimeFileValid);
  KT_RUN_TEST(IoDimFileValid);

  KT_RUN_TEST(IoCtrlFileValid);

  return KT_TEST_SUCCESS;
}
