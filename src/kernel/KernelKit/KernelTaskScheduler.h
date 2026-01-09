// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef KERNELKIT_KERNELTASKSCHEDULER_H
#define KERNELKIT_KERNELTASKSCHEDULER_H

/// @file KernelTaskScheduler.h
/// @brief Kernel Task Scheduler backend file.
/// @author Amlal El Mahrouss (amlal@nekernel.org)

#include <ArchKit/ArchKit.h>
#include <KernelKit/CoreProcessScheduler.h>
#include <KernelKit/LockDelegate.h>

namespace Kernel {

class KernelTaskHelper;

using KID = ProcessID;

/// @brief Equivalent of UserProcess, but for kernel tasks.
/// @author Amlal
class KernelTask {
 public:
  Char               Name[kSchedNameLen] = {"KernelTask"};
  ProcessSubsystem   SubSystem{ProcessSubsystem::kProcessSubsystemKernel};
  HAL::StackFramePtr StackFrame{nullptr};
  UInt8*             StackReserve{nullptr};
  SizeT              StackSize{kSchedMaxStackSz};
  ProcessImage       Image{};

  /// @brief a KID is a Kernel ID, it is used to find a task running within the kernel.
  KID Kid{};

  NE_NON_VETTABLE;
};

/// @brief Equivalent of UserProcessHelper, but for kernel tasks.
/// @author Amlal
class KernelTaskHelper final {
 public:
  STATIC Bool Add(HAL::StackFramePtr frame_ptr, KID kid);
  STATIC Bool Remove(const KID kid);
  STATIC Bool CanBeScheduled(const KernelTask& process);
  STATIC ErrorOr<KID> TheCurrentKID();
  STATIC SizeT        StartScheduling();
};

}  // namespace Kernel

#endif
