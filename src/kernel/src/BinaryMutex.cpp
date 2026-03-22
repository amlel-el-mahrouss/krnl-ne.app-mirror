// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <KernelKit/BinaryMutex.h>
#include <KernelKit/ProcessScheduler.h>

namespace Kernel {
/***********************************************************************************/
/// @brief Unlocks the binary mutex.
/***********************************************************************************/

#ifndef __NE_TIMEOUT_CONFIG__
#define __NE_TIMEOUT_CONFIG__ 10000
#endif

Bool BinaryMutex::Unlock() {
  auto timeout = 0UL;
  constexpr auto kTimoutLimit = __NE_TIMEOUT_CONFIG__;

  while (fLockingProcess->Status == ProcessStatusKind::kRunning) {
    ++timeout;

    if (timeout > kTimoutLimit)
      return No;
  }

  fLockingProcess = nullptr;
  return Yes;
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
  this->Lock(process);
  return this->Unlock();
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
