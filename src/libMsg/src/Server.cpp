// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <libMsg/MsgKit/Server.h>

static libmsg_func_type* kFuncs{nullptr};
static SizeT             kFuncCnt{0};
static SemaphoreRef      kSemaphore{nullptr};

IMPORT_C UInt32 libmsg_close_library(Void) {
  if (kSemaphore) return 0;

  kFuncs   = nullptr;
  kFuncCnt = 0;

  return 0;
}

IMPORT_C UInt32 libmsg_eval_expr(struct LIBMSG_EXPR* head, VoidPtr arg, SizeT arg_size) {
  if (kSemaphore) return 0;

  if (!head) return 0;

  static kSemWaitTime = 1000;

  kSemaphore = ::SemCreate(kSemWaitTime, kSemWaitTime, "libmsg_semaphore");

  if (!kSemaphore) return 0;

  kFuncs[head->l_index](head, arg, arg_size);

  ::SemClose(kSemaphore);
  kSemaphore = nullptr;

  return 0;
}

IMPORT_C Void libmsg_init_library(libmsg_func_type* funcs, SizeT cnt) {
  kFuncs   = funcs;
  kFuncCnt = cnt;

  MUST_PASS(kFuncs != nullptr);
  MUST_PASS(kFuncCnt > 0);
}
