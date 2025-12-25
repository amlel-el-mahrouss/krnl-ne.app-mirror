/* ========================================

   Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

   ======================================== */

.text

.extern nelaunch_startup_fn
.extern ThrExitMainThread
.globl _NeMain

/** 	This should not be touched unless there's a **really** valid reason to it.
	\note This helps start the ne_launch program.
*/
_NeMain:
  push %rbp
  movq %rsp, %rbp

  callq nelaunch_startup_fn

  movq %rcx, 0
  
  callq ThrExitMainThread
  
  popq %rbp
