/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

  FILE: UserProcessScheduler.inl
  PURPOSE: Low level/Ring-3 process scheduler.

======================================== */

/// @brief UPS inline definitions.
/// @author Amlal El Mahrouss (amlal@nekernel.org)
/// @date Tue Apr 22 22:01:07 CEST 2025

#ifndef __KERNEL_KIT_USER_PROCESS_SCHEDULER_H__
#include <KernelKit/UserProcessScheduler.h>
#endif  // __KERNEL_KIT_USER_PROCESS_SCHEDULER_H__

namespace Kernel {
/***********************************************************************************/
/** @brief Free pointer/file from usage. */
/***********************************************************************************/

template <typename T>
BOOL UserProcess::Delete(ErrorOr<T*> ptr) {
  if (!ptr) return No;

  if (!this->HeapTree) {
    kout << "UserProcess: Heap is empty.\r";
    return No;
  }

  ProcessHeapTree<VoidPtr>* entry = this->HeapTree;

  while (entry != nullptr) {
    if (entry->Entry == ptr.Leak().Leak()) {
      this->UsedMemory -= entry->EntrySize;

#ifdef __NE_AMD64__
      auto page_dir = hal_read_cr3();

      hal_write_cr3(this->VMRegister);
      auto ret = mm_free_ptr(entry->Entry);
      hal_write_cr3(page_dir);
#else
      auto ret = mm_free_ptr(ptr.Leak().Leak());
#endif

      return ret == kErrorSuccess;
    }

    entry = entry->Next;
  }

  kout << "UserProcess: Trying to free a pointer which doesn't exist.\r";

  this->Crash();

  return No;
}
}  // namespace Kernel
