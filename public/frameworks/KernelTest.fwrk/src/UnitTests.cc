/* -------------------------------------------

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

------------------------------------------- */

#include <public/frameworks/KernelTest.fwrk/headers/KernelTest.h>

EXTERN_C Kernel::Void KT_TestBreak() {
  KT_ALWAYS_BREAK brk;
  brk.Run();
}

EXTERN_C Kernel::Void KT_TestGood() {
  KT_ALWAYS_GOOD good;
  good.Run();
}