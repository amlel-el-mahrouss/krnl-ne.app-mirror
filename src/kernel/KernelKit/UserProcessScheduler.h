/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef __KERNEL_KIT_USER_PROCESS_SCHEDULER_H__
#define __KERNEL_KIT_USER_PROCESS_SCHEDULER_H__

/// @file UserProcessScheduler.h
/// @brief User Process Scheduler backend file.
/// @author Amlal El Mahrouss (amlal@nekernel.org)

#include <ArchKit/ArchKit.h>
#include <KernelKit/CoreProcessScheduler.h>
#include <KernelKit/LockDelegate.h>
#include <KernelKit/UserMgr.h>
#include <NeKit/MutableArray.h>

////////////////////////////////////////////////////
// Last revision date is: Fri Mar 28 2025		  //
////////////////////////////////////////////////////

namespace Kernel {
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

 public:
  Char               Name[kSchedNameLen] = {"UserProcess"};
  ProcessSubsystem   SubSystem{ProcessSubsystem::kProcessSubsystemUser};
  User*              Owner{nullptr};
  HAL::StackFramePtr StackFrame{nullptr};
  AffinityKind       Affinity{AffinityKind::kStandard};
  ProcessStatusKind  Status{ProcessStatusKind::kKilled};
  UInt8              StackReserve[kSchedMaxStackSz];
  ProcessImage       Image{};
  SizeT              StackSize{kSchedMaxStackSz};
  IDylibObject*      DylibDelegate{nullptr};
  SizeT              MemoryCursor{0UL};
  SizeT              MemoryLimit{kSchedMaxMemoryLimit};
  SizeT              UsedMemory{0UL};

  struct UserProcessSignal {
    UIntPtr           SignalArg{0};
    ProcessStatusKind Status{ProcessStatusKind::kKilled};
    UIntPtr           SignalID{0};
  };

  UserProcessSignal         Signal;
  ProcessFileTree<VoidPtr>* FileTree{nullptr};
  ProcessHeapTree<VoidPtr>* HeapTree{nullptr};
  UserProcessTeam*          ParentTeam;

 public:
  using VMReg = VoidPtr;

  VoidPtr VMRegister{0UL};

  enum struct ExecutableKind {
    kInvalidExecutableKind,
    kExecutableKind,
    kExecutableDylibKind,
    kExecutableKindCount,
  };

  ProcessTime PTime{0};  //! @brief Process allocated tine.
  ProcessTime RTime{0};  //! @brief Process run time.
  ProcessTime UTime{0};  //! #brief Process used time.

  ProcessID      ProcessId{kSchedInvalidPID};
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

  const User* GetOwner();

  const ProcessStatusKind& GetStatus();

  const AffinityKind& GetAffinity();

 private:
  KPCError LastExitCode{0};
  KPCError LocalCode{0};

  friend UserProcessScheduler;
  friend UserProcessHelper;
};

using UserProcessArray = Array<UserProcess, kSchedProcessLimitPerTeam>;
using UserProcessRef   = Ref<UserProcess>;

/// \brief Processs Team (contains multiple processes inside it.)
/// Equivalent to a process batch
class UserProcessTeam final {
 public:
  explicit UserProcessTeam();
  ~UserProcessTeam() = default;

  NE_COPY_DEFAULT(UserProcessTeam)

  Array<UserProcess, kSchedProcessLimitPerTeam>& AsArray();
  Ref<UserProcess>&                              AsRef();
  ProcessID&                                     Id();

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
  Ref<UserProcessTeam> TheCurrentTeam();
  BOOL                 SwitchTeam(UserProcessTeam& team);

 public:
  ProcessID Spawn(const Char* name, VoidPtr code, VoidPtr image);
  Void      Remove(ProcessID process_id);

  Bool IsUser() override;
  Bool IsKernel() override;
  Bool HasMP() override;

 public:
  Ref<UserProcess> TheCurrentProcess();
  SizeT            Run();

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
}  // namespace Kernel

#include <KernelKit/ThreadLocalStorage.h>
#include <KernelKit/UserProcessScheduler.inl>

////////////////////////////////////////////////////
// END
////////////////////////////////////////////////////

#endif /* ifndef __KERNEL_KIT_USER_PROCESS_SCHEDULER_H__ */
