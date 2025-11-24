/* ========================================

   Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

   ======================================== */

.text

.extern nelaunch_startup_fn
.extern ThrExitMainThread
.globl _NeMain

_NeMain:
  push %rbp
  movq %rsp, %rbp

  callq nelaunch_startup_fn

  movq %rcx, 0
  
  callq ThrExitMainThread
  
  popq %rbp
