// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <KernelKit/BinaryMutex.h>
#include <KernelKit/ProcessScheduler.h>

namespace Kernel {
/***********************************************************************************/
/// @brief Unlocks the binary mutex.
/***********************************************************************************/

Bool BinaryMutex::Unlock() {
  if (fLockingProcess->Status == ProcessStatusKind::kRunning) {
    fLockingProcess = nullptr;

    return Yes;
  }

  return No;
}

/***********************************************************************************/
/// @brief Locks process in the binary mutex.
/***********************************************************************************/

Bool BinaryMutex::Lock(BinaryMutex::LockedPtr process) {
  if (!process || this->IsLocked()) return No;

  this->fLockingProcess = process;

  return Yes;
}

/***********************************************************************************/
/// @brief Checks if process is locked.
/***********************************************************************************/

Bool BinaryMutex::IsLocked() const {
  return this->fLockingProcess && this->fLockingProcess->Status == ProcessStatusKind::kRunning;
}

/***********************************************************************************/
/// @brief Try lock or wait.
/***********************************************************************************/

Bool BinaryMutex::LockAndWait(BinaryMutex::LockedPtr process, ITimer* timer) {
  if (timer == nullptr) return No;

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
