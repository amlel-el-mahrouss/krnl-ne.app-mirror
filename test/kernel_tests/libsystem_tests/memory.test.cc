/// \file memory.test.cc
/// \brief Memory Manager API tests.
/// \author Amlal El Mahrouss (amlal at nekernel dot org)

#include <SystemKit/System.h>
#include <public/frameworks/KernelTest/headers/TestCase.h>

/// \note Memory allocation tests

KT_DECL_TEST(MmCreateHeapSuccess, []() -> bool {
  auto heap = MmCreateHeap(1024, 0);
  if (!heap) return NO;
  MmDestroyHeap(heap);
  return YES;
});

KT_DECL_TEST(MmCreateHeapZeroSize, []() -> bool {
  auto heap = MmCreateHeap(0, 0);
  return heap == nullptr;
});

KT_DECL_TEST(MmDestroyHeapValid, []() -> bool {
  auto heap = MmCreateHeap(1024, 0);
  if (!heap) return NO;
  return MmDestroyHeap(heap) == 0;
});

KT_DECL_TEST(MmDestroyHeapNull, []() -> bool { return MmDestroyHeap(nullptr) != 0; });

/// \note Memory operations tests

KT_DECL_TEST(MmCopyMemoryNonOverlapping, []() -> bool {
  char src[16] = "Hello, World!";
  char dst[16] = {0};

  auto result = MmCopyMemory(dst, src, 13);
  if (!result) return NO;

  return MmStrCmp(src, dst) == 0;
});

KT_DECL_TEST(MmCopyMemoryNull,
             []() -> bool { return MmCopyMemory(nullptr, nullptr, 10) == nullptr; });

KT_DECL_TEST(MmFillMemoryPattern, []() -> bool {
  char buffer[16] = {0};
  MmFillMemory(buffer, 16, 0xAA);

  for (int i = 0; i < 16; i++) {
    if ((unsigned char) buffer[i] != 0xAA) return NO;
  }
  return YES;
});

KT_DECL_TEST(MmFillMemoryNull, []() -> bool { return MmFillMemory(nullptr, 10, 0xFF) == nullptr; });

KT_DECL_TEST(MmCmpMemoryEqual, []() -> bool {
  char buf1[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  char buf2[8] = {1, 2, 3, 4, 5, 6, 7, 8};

  return MmCmpMemory(buf1, buf2, 8) == 0;
});

KT_DECL_TEST(MmCmpMemoryNotEqual, []() -> bool {
  char buf1[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  char buf2[8] = {1, 2, 3, 9, 5, 6, 7, 8};

  return MmCmpMemory(buf1, buf2, 8) != 0;
});

/// \note String operations tests

KT_DECL_TEST(MmStrLenNonEmpty, []() -> bool {
  const char* str = "Hello";
  return MmStrLen(str) == 5;
});

KT_DECL_TEST(MmStrLenEmpty, []() -> bool {
  const char* str = "";
  return MmStrLen(str) == 0;
});

KT_DECL_TEST(MmStrLenNull, []() -> bool { return MmStrLen(nullptr) < 0; });

KT_DECL_TEST(MmStrCmpEqual, []() -> bool { return MmStrCmp("test", "test") == 0; });

KT_DECL_TEST(MmStrCmpNotEqual, []() -> bool { return MmStrCmp("test", "fest") != 0; });

/// \note Heap flags tests

KT_DECL_TEST(MmSetGetHeapFlags, []() -> bool {
  auto heap = MmCreateHeap(1024, 0x01);
  if (!heap) return NO;

  MmSetHeapFlags(heap, 0x02);
  UInt32 flags = MmGetHeapFlags(heap);

  MmDestroyHeap(heap);
  return flags == 0x02;
});

KT_DECL_TEST(MmFillCRC32HeapValid, []() -> bool {
  auto heap = MmCreateHeap(1024, 0);
  if (!heap) return NO;

  UInt32 crc = MmFillCRC32Heap(heap);

  MmDestroyHeap(heap);
  return crc != 0;
});

/// \brief Run memory tests.
IMPORT_C SInt32 KT_TEST_MAIN() {
  KT_RUN_TEST(MmCreateHeapSuccess);
  KT_RUN_TEST(MmCreateHeapZeroSize);
  KT_RUN_TEST(MmDestroyHeapValid);
  KT_RUN_TEST(MmDestroyHeapNull);

  KT_RUN_TEST(MmCopyMemoryNonOverlapping);
  KT_RUN_TEST(MmCopyMemoryNull);
  KT_RUN_TEST(MmFillMemoryPattern);
  KT_RUN_TEST(MmFillMemoryNull);
  KT_RUN_TEST(MmCmpMemoryEqual);
  KT_RUN_TEST(MmCmpMemoryNotEqual);

  KT_RUN_TEST(MmStrLenNonEmpty);
  KT_RUN_TEST(MmStrLenEmpty);
  KT_RUN_TEST(MmStrLenNull);
  KT_RUN_TEST(MmStrCmpEqual);
  KT_RUN_TEST(MmStrCmpNotEqual);

  KT_RUN_TEST(MmSetGetHeapFlags);
  KT_RUN_TEST(MmFillCRC32HeapValid);

  return KT_TEST_SUCCESS;
}
