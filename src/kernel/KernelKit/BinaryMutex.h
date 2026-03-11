// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel


#ifndef KERNELKIT_BINARYMUTEX_H
#define KERNELKIT_BINARYMUTEX_H

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

#endif
