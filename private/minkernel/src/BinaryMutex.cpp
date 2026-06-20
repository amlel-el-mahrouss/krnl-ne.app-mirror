// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <KernelKit/BinaryMutex.h>
#include <KernelKit/ProcessScheduler.h>

namespace Kernel {

Bool BinaryMutex::Unlock() {
  if (!fLockingProcess) return No;

  // restore original priority if we boosted the owner
  if (fOwnerOriginalAffinity != AffinityKind::kInvalid) {
    fLockingProcess->Affinity = fOwnerOriginalAffinity;
    fOwnerOriginalAffinity    = AffinityKind::kInvalid;
  }

  fLockingProcess = nullptr;
  return Yes;
}

/***********************************************************************************/
/// @brief Locks process in the binary mutex.
/***********************************************************************************/

Bool BinaryMutex::Lock(BinaryMutex::LockedPtr process) {
  if (!process) return No;

  // if already locked, implement priority inheritance
  if (this->IsLocked() && fLockingProcess) {
    // boost owner to waiter's priority if waiter is higher priority (lower value = higher priority)
    if (process->Affinity < fLockingProcess->Affinity) {
      fOwnerOriginalAffinity    = fLockingProcess->Affinity;
      fLockingProcess->Affinity = process->Affinity;
    }
    return No;  // lock not acquired, but owner boosted
  }

  this->fLockingProcess  = process;
  fOwnerOriginalAffinity = AffinityKind::kInvalid;

  return Yes;
}

/***********************************************************************************/
/// @brief Checks if process is locked.
/***********************************************************************************/

Bool BinaryMutex::IsLocked() const {
  return this->fLockingProcess && this->fLockingProcess->Status == ProcessStatusKind::kRunning;
}

/***********************************************************************************/
/// @brief Try lock, waiting until timeout if already locked.
/***********************************************************************************/

Bool BinaryMutex::LockAndWait(BinaryMutex::LockedPtr process, ITimer* timer) {
  if (timer == nullptr || !process) return No;

  // try to acquire lock immediately
  if (this->Lock(process)) return Yes;

  // wait and retry
  timer->Wait();
  return this->Lock(process);
}

/***********************************************************************************/
/// @brief Wait for process **sec** until we check if it's free.
/// @param sec seconds.
/***********************************************************************************/

BOOL BinaryMutex::WaitForProcess(const UInt32& sec) {
  HardwareTimer hw_timer(rtl_milliseconds(sec));
  hw_timer.Wait();

  return !this->IsLocked();
}
}  // namespace Kernel
