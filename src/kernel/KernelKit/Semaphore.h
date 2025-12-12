/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef __KERNEL_KIT_TLS_H__
#define __KERNEL_KIT_TLS_H__

/// @author Amlal El Mahrouss
/// @file Semaphore.h
/// @brief Semaphore structure and functions for synchronization in the kernel.

#include <CompilerKit/CompilerKit.h>
#include <KernelKit/Timer.h>
#include <NeKit/Config.h>

#define kSemaphoreOwnerIndex (0U)
#define kSemaphoreCountIndex (1U)

#define kSemaphoreCount (2U)

#define kSemaphoreIncrementOwner(sem) (sem[kSemaphoreOwnerIndex]++)
#define kSemaphoreDecrementOwner(sem) (sem[kSemaphoreOwnerIndex]--)

namespace Kernel {
/// @brief Semaphore structure used for synchronization.
using SemaphoreArr = UInt64[kSemaphoreCount];

/// @brief Checks if the semaphore is valid.
inline bool rtl_sem_is_valid(const SemaphoreArr& sem, UInt64 owner = 0) {
  return sem[kSemaphoreOwnerIndex] == owner && sem[kSemaphoreCountIndex] > 0;
}

/// @brief Releases the semaphore, resetting its owner and count.
/// @param sem
/// @return
inline bool rtl_sem_release(SemaphoreArr& sem) {
  sem[kSemaphoreOwnerIndex] = 0;
  sem[kSemaphoreCountIndex] = 0;

  return true;
}

/// @brief Initializes the semaphore with an owner and a count of zero.
/// @param sem the semaphore array to use.
/// @param owner the owner to set, could be anything identifitable.
/// @return
inline bool rtl_sem_acquire(SemaphoreArr& sem, const UInt64 owner) {
  if (!owner) {
    err_global_get() = kErrorInvalidData;
    return false;  // Invalid owner
  }

  sem[kSemaphoreOwnerIndex] = owner;
  sem[kSemaphoreCountIndex] = 0;

  return true;
}

/// @brief Waits for the semaphore to be available, blocking until it is.
/// @param sem
/// @param timeout
/// @param condition condition pointer.
/// @return
inline bool rtl_sem_wait(SemaphoreArr& sem, const UInt64 owner, const UInt64 timeout,
                         bool& condition) {
  if (!rtl_sem_is_valid(sem, owner)) {
    return false;
  }

  if (timeout < 1) {
    err_global_get() = kErrorTimeout;

    return false;
  }

  if (!condition) {
    if (sem[kSemaphoreCountIndex] == 0) {
      err_global_get() = kErrorUnavailable;
      return false;
    }

    err_global_get() = kErrorSuccess;
    sem[kSemaphoreCountIndex]--;

    return true;
  }

  HardwareTimer timer(timeout);
  bool          ret = timer.Wait();

  if (ret) {
    if (!condition) {
      if (sem[kSemaphoreCountIndex] == 0) {
        err_global_get() = kErrorUnavailable;
        return false;
      }

      err_global_get() = kErrorSuccess;
      sem[kSemaphoreCountIndex]--;

      return true;
    }
  }

  err_global_get() = kErrorTimeout;

  return false;  // Failed to acquire semaphore
}
}  // namespace Kernel

#endif  // !__KERNEL_KIT_TLS_H__