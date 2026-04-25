// SPDX-License-Identifier: Apache-2.0
// Copyright 2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <libPThread/PThreadKit/Thread.h>
#include <libSystem/SystemKit/Err.h>

PTHREAD_SAFE SInt32 pthread_detach(ThreadRef thread) {
  return ::ThrDetachThread(thread);
}

PTHREAD_SAFE SInt32 pthread_yield(void) {
  return ::ThrYieldThread(::ThrCurrentThread());
}

PTHREAD_SAFE ThreadRef pthread_self(void) {
  return ::ThrCurrentThread();
}

PTHREAD_SAFE SInt32 pthread_exit(SInt32 retval) {
  return ::ThrExitCurrentThread(retval);
}

PTHREAD_SAFE SInt32 pthread_join(ThreadRef thread, VoidPtr* retval) {
  SInt32* ret = (SInt32*) retval;

  if (!ret) return kErrorInvalidData;

  *ret = ::ThrJoinThread(thread);
  return kErrorSuccess;
}

PTHREAD_SAFE SInt32 pthread_create(_Output ThreadRef* thread, VoidPtr         attr,
                                   VoidPtr (*start_routine)(VoidPtr), VoidPtr arg) {
  LIBSYS_UNUSED(attr);

  if (!attr || !thread || !arg || !start_routine) return kErrorInvalidData;

  /// @note passing zero means you'd have to read the argv until you hit a nullptr.
  ThreadRef thrd = ::ThrCreateThread(nullptr, (ThrProcKind) start_routine, 1, arg, 0);

  if (!thrd) return kErrorInvalidData;

  *thread = thrd;
  return kErrorSuccess;
}
