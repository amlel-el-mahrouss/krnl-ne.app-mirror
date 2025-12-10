/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <CompilerKit/CompilerKit.h>
#include <KernelKit/Timer.h>
#include <NeKit/Config.h>

namespace Kernel {
class UserProcess;

/// @brief Access control class, which locks a task until one is done.
class BinaryMutex final {
 public:
  using LockedPtr = UserProcess*;

  explicit BinaryMutex() = default;
  ~BinaryMutex()         = default;

 public:
  bool IsLocked() const;
  bool Unlock();

 public:
  BOOL WaitForProcess(const UInt32& sec);

 public:
  bool Lock(LockedPtr process);
  bool LockAndWait(LockedPtr process, ITimer* timer);

 public:
  NE_COPY_DEFAULT(BinaryMutex)

 private:
  LockedPtr fLockingProcess{nullptr};
};
}  // namespace Kernel
