// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef KERNELKIT_USERPROCESSSCHEDULER_H
#define KERNELKIT_USERPROCESSSCHEDULER_H

/// @file UserProcessScheduler.h
/// @brief User Process Scheduler backend file.
/// @author Amlal El Mahrouss (amlal@nekernel.org)

#include <ArchKit/ArchKit.h>
#include <KernelKit/CoreProcessScheduler.h>
#include <KernelKit/LockDelegate.h>
#include <KernelKit/User.h>
#include <NeKit/MutableArray.h>

////////////////////////////////////////////////////
// Last revision date is: Fri Mar 28 2025		  //
////////////////////////////////////////////////////

namespace Ne::Kernel {

//! @brief Forward declarations.

class IDylibObject;
class UserProcessScheduler;
class UserProcessHelper;

/***********************************************************************************/
/// @name UserProcess
/// @brief UserProcess class, holds information about the running process/thread.
/***********************************************************************************/
class UserProcess final {
 public:
  UserProcess();
  ~UserProcess();

 public:
  NE_COPY_DEFAULT(UserProcess)

  using TypeRef   = UserProcess&;
  using ConstType = const UserProcess&;
  using TypePtr   = UserProcess*;

  NE_VETTABLE;

  struct UserProcessSignal {
    UIntPtr           SignalArg{0};
    ProcessStatusKind Status{ProcessStatusKind::kKilled};
    UIntPtr           SignalID{0};
  };

  Char               Name[kCPSNameLen] = {"UserProcess"};
  ProcessSubsystem   SubSystem{ProcessSubsystem::kProcessSubsystemUser};
  User*              Owner{nullptr};
  HAL::StackFramePtr StackFrame{nullptr};
  AffinityKind       Affinity{AffinityKind::kStandard};
  ProcessStatusKind  Status{ProcessStatusKind::kKilled};
  UInt8              StackReserve[kCPSMaxStackSz];
  SizeT              StackSize{kCPSMaxStackSz};
  IDylibObject*      DylibDelegate{nullptr};
  SizeT              MemoryCursor{0UL};
  SizeT              MemoryLimit{kCPSMaxMemoryLimit};
  SizeT              UsedMemory{0UL};  /// AMLALE: This should be thread-safe as well.
  SizeT UsedFiles{0UL};  /// AMLALE: This is a thread-safe counter to track file allocation count.
  UserProcessSignal Signal;
  ProcessImage      Image;

 private:
  ProcessFileTree<Any>* FileTree{nullptr};
  ProcessHeapTree<Any>* HeapTree{nullptr};
  UserProcessTeam*      Parent;

 public:
  Any VMRegister{0UL};

  enum struct ExecutableKind {
    kInvalidExecutableKind,
    kExecutableKind,
    kExecutableDylibKind,
    kExecutableKindCount,
  };

  ProcessTime PTime{0};  //! @brief Process allocated time.
  ProcessTime RTime{0};  //! @brief Process run time.
  ProcessTime UTime{0};  //! @brief Process used time.
  ProcessTime STime{0};  //! @brief Process sleep time (for dynamic priority boost).

  ProcessID      ProcessId{kCPSInvalidPID};
  ExecutableKind Kind{ExecutableKind::kExecutableKind};

 public:
  /***********************************************************************************/
  //! @brief boolean operator, check status.
  /***********************************************************************************/
  explicit operator bool();

  /***********************************************************************************/
  ///! @brief Crashes the app, exits with code ~0.
  /***********************************************************************************/
  Void Crash();

  /***********************************************************************************/
  ///! @brief Gets the parent team of the process.
  ///! @return The parent team.
  /***********************************************************************************/
  UserProcessTeam* GetParentTeam();

  /***********************************************************************************/
  ///! @brief Spawns a dynamic library handle if dylib.
  /***********************************************************************************/
  Bool InitDylib();

  /***********************************************************************************/
  ///! @brief Exits the app.
  /***********************************************************************************/
  Void Exit(const Int32& exit_code = 0);

  /***********************************************************************************/
  ///! @brief TLS allocate.
  ///! @param sz size of data structure.
  ///! @param pad_amount amount to add after pointer.
  ///! @return A wrapped pointer, or error code.
  /***********************************************************************************/
  ErrorOr<VoidPtr> New(SizeT sz, SizeT pad_amount = 0);

  /***********************************************************************************/
  ///! @brief TLS free.
  ///! @param ptr the pointer to free.
  ///! @param sz the size of it.
  /***********************************************************************************/
  template <typename T>
  Boolean Delete(ErrorOr<T*> ptr);

  /***********************************************************************************/
  ///! @brief Wakes up thread.
  /***********************************************************************************/
  Void Wake(Bool wakeup = false);

 public:
  /***********************************************************************************/
  //! @brief Gets the local exit code.
  /***********************************************************************************/
  KPCError& GetExitCode();

  /***********************************************************************************/
  ///! @brief Get the process's name
  ///! @example process can be called 'C Runtime Library'.
  /***********************************************************************************/
  const Char* GetName();

  /***********************************************************************************/
  //! @brief return local error code of process.
  //! @return Int32 local error code.
  /***********************************************************************************/
  KPCError& GetLocalCode();

  const Ref<User*> GetOwner();

 private:
  KPCError LastExitCode{0};
  KPCError LocalCode{0};

  friend UserProcessScheduler;
  friend UserProcessHelper;
};

inline bool operator<(UserProcess::ExecutableKind lhs, UserProcess::ExecutableKind rhs) {
  Int32 lhs_int = static_cast<Int32>(lhs);
  Int32 rhs_int = static_cast<Int32>(rhs);

  return lhs_int < rhs_int;
}

inline bool operator>(UserProcess::ExecutableKind lhs, UserProcess::ExecutableKind rhs) {
  Int32 lhs_int = static_cast<Int32>(lhs);
  Int32 rhs_int = static_cast<Int32>(rhs);

  return lhs_int > rhs_int;
}

inline bool operator<=(UserProcess::ExecutableKind lhs, UserProcess::ExecutableKind rhs) {
  Int32 lhs_int = static_cast<Int>(lhs);
  Int32 rhs_int = static_cast<Int32>(rhs);

  return lhs_int <= rhs_int;
}

inline bool operator>=(UserProcess::ExecutableKind lhs, UserProcess::ExecutableKind rhs) {
  Int32 lhs_int = static_cast<Int32>(lhs);
  Int32 rhs_int = static_cast<Int32>(rhs);

  return lhs_int >= rhs_int;
}

using UserProcessArray = Array<UserProcess, kCPSProcessLimitPerTeam>;
using UserProcessRef   = Ref<UserProcess>;

/// \brief Processs Team (contains multiple processes inside it.)
/// Equivalent to a process batch
class UserProcessTeam final {
 public:
  explicit UserProcessTeam();
  ~UserProcessTeam() = default;

  NE_COPY_DEFAULT(UserProcessTeam)

  Array<UserProcess, kCPSProcessLimitPerTeam>& AsArray();
  Ref<UserProcess>&                            AsRef();
  ProcessID&                                   Id();

 public:
  UserProcessArray mProcessList;
  UserProcessRef   mCurrentProcess;
  ProcessID        mTeamId{0};
  ProcessID        mProcessCur{0};
};

/***********************************************************************************/
/// @brief UserProcess scheduler class.
/// The main class which you call to schedule user processes.
/***********************************************************************************/
class UserProcessScheduler final : public ISchedulable {
  friend class UserProcessHelper;

 public:
  explicit UserProcessScheduler()  = default;
  ~UserProcessScheduler() override = default;

  NE_COPY_DELETE(UserProcessScheduler)
  NE_MOVE_DELETE(UserProcessScheduler)

 public:
  explicit operator bool();
  bool     operator!();

 public:
  UserProcessTeam&     TheCurrentTeam();
  BOOL                 SwitchTeam(UserProcessTeam& team);

 public:
  ProcessID Spawn(const Char* name, VoidPtr code, VoidPtr image, SizeT image_sz = 0UL);
  Void      Remove(ProcessID process_id);

  Bool IsUser() override;
  Bool IsKernel() override;
  Bool HasMP() override;

 public:
  UserProcess& TheCurrentProcess();
  SizeT        Run();

 public:
  STATIC UserProcessScheduler& The();

 private:
  UserProcessTeam mTeam{};
};

/***********************************************************************************/
/**
 * \brief UserProcess helper class, which contains needed utilities for the scheduler.
 */
/***********************************************************************************/

class UserProcessHelper final {
 public:
  STATIC Bool Switch(HAL::StackFramePtr frame_ptr, ProcessID new_pid);
  STATIC Bool CanBeScheduled(const UserProcess& process);
  STATIC ErrorOr<ProcessID> TheCurrentPID();
  STATIC SizeT              StartScheduling();
};

}  // namespace Ne::Kernel

#include <KernelKit/ThreadLocalStorage.h>
#include "UserProcessScheduler.inl"

////////////////////////////////////////////////////
// END
////////////////////////////////////////////////////

#endif /* ifndef KERNELKIT_USERPROCESSSCHEDULER_H */
