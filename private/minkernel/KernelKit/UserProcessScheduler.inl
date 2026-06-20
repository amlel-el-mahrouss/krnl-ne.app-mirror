// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

/// @brief UPS inline definitions.
/// @author Amlal El Mahrouss (amlal@nekernel.org)
/// @date Tue Apr 22 22:01:07 CEST 2025

#ifndef KERNELKIT_USERPROCESSSCHEDULER_H
#include <KernelKit/UserProcessScheduler.h>
#endif  // KERNELKIT_USERPROCESSSCHEDULER_H

namespace Kernel {

/***********************************************************************************/
/** @brief Free pointer/file from usage. */
/***********************************************************************************/

template <class T>
BOOL UserProcess::Delete(ErrorOr<T*> ptr) {
  if (!ptr) {
    kout << "UserProcess: Ptr is nils.\r";
    return No;
  }

  if (!this->HeapTree) {
    kout << "UserProcess: Heap is empty.\r";
    return No;
  }

  ProcessHeapTree<Any>* entry = this->HeapTree;

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

  (Void)(kout << "UserProcess: Pointer not found in heap." << kendl);

  // crash.
  this->Crash();

  return No;
}

}  // namespace Kernel
