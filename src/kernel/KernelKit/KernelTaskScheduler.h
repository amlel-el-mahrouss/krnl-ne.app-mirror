/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

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
class KernelTask final {
 public:
  Char               Name[kSchedNameLen] = {"KernelTask"};
  ProcessSubsystem   SubSystem{ProcessSubsystem::kProcessSubsystemKernel};
  HAL::StackFramePtr StackFrame{nullptr};
  UInt8*             StackReserve{nullptr};
  SizeT              StackSize{kSchedMaxStackSz};
  ProcessImage       Image{};
  /// @brief a KID is a Kernel ID, it is used to find a task running within
  /// the kernel.
  KID Kid{0};

  NE_NON_VETTABLE;
};

/// @brief Equivalent of UserProcessHelper, but for kernel tasks.
/// @author Amlal
class KernelTaskHelper final {
 public:
  STATIC Bool Add(HAL::StackFramePtr frame_ptr, ProcessID new_kid);
  STATIC Bool Remove(const KID kid);
  STATIC Bool CanBeScheduled(const KernelTask& process);
  STATIC ErrorOr<KID> TheCurrentKID();
  STATIC SizeT        StartScheduling();
};
}  // namespace Kernel