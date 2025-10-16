/* -------------------------------------------

   Copyright (C) 2025, Amlal El Mahrouss, all rights reserved.

   ------------------------------------------- */

.text

.extern nelaunch_startup_fn
.extern ThrExitMainThread
.globl _NeMain

_NeMain:
  push %rbp
  movq %rsp, %rbp
  callq nelaunch_startup_fn
  xor %rax, %rax
  popq %rbp

  push %rbp
  movq %rsp, %rbp
  movq %rcx, 0
  callq ThrExitMainThread
  popq %rbp

  retq
