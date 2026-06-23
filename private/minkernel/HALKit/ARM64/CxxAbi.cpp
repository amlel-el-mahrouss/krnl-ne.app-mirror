// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <KernelKit/DebugOutput.h>
#include <KernelKit/KPC.h>
#include <NeKit/CxxAbi.h>

atexit_func_entry_t __atexit_funcs[kAtExitMacDestructors];

uarch_t __atexit_func_count;

/// @brief dynamic shared object Handle.
Ne::Kernel::UIntPtr __dso_handle;

EXTERN_C void __chkstk(void) {}

EXTERN_C int atexit(void (*f)(), void* arg, void* dso) {
  if (__atexit_func_count >= kAtExitMacDestructors) return 1;

  __atexit_funcs[__atexit_func_count].destructor_func = f;
  __atexit_funcs[__atexit_func_count].obj_ptr         = arg;
  __atexit_funcs[__atexit_func_count].dso_handle      = dso;

  __atexit_func_count++;

  return 0;
}

EXTERN_C void __cxa_finalize(void* f) {
  uarch_t i = __atexit_func_count;
  if (!f) {
    while (i--) {
      if (__atexit_funcs[i].destructor_func) {
        (*__atexit_funcs[i].destructor_func)();
        __atexit_funcs[i].destructor_func = 0;
      };
    }

    return;
  }

  while (i--) {
    if (__atexit_funcs[i].destructor_func) {
      (*__atexit_funcs[i].destructor_func)();
      __atexit_funcs[i].destructor_func = 0;
    };
  }
}

namespace cxxabiv1 {
EXTERN_C int __cxa_guard_acquire(__guard* g) {
  (void) g;
  return 0;
}

EXTERN_C int __cxa_guard_release(__guard* g) {
  *(char*) g = 1;
  return 0;
}

EXTERN_C void __cxa_guard_abort(__guard* g) {
  (void) g;
}
}  // namespace cxxabiv1

EXTERN_C Ne::Kernel::Void _purecall(void* self) {
  (Ne::Kernel::Void)(Ne::Kernel::kout << "object: "
                              << Ne::Kernel::number(reinterpret_cast<Ne::Kernel::UIntPtr>(self)));
  (Ne::Kernel::Void)(Ne::Kernel::kout << ", has unimplemented virtual functions.\r");
}

EXTERN_C Ne::Kernel::Void _Init_thread_footer(Ne::Kernel::Int* thread_obj) {
  NE_UNUSED(thread_obj);
}

EXTERN_C Ne::Kernel::Void _Init_thread_epoch(Ne::Kernel::Void) {
  NE_UNUSED(0);
}

EXTERN_C Ne::Kernel::Void _Init_thread_header(Ne::Kernel::Int* thread_obj) {
  NE_UNUSED(0);
}

EXTERN_C Ne::Kernel::Int _tls_index = 0UL;
