// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

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
