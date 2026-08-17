// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef KERNELKIT_KPC_H
#define KERNELKIT_KPC_H

#include <NeKit/Config.h>

/// @file KPC.h
/// @brief Ne::Kernel Procedure Code.

#define err_local_ok() \
  (Ne::Kernel::UserProcessScheduler::The().TheCurrentProcess().GetLocalCode() == Ne::Kernel::kErrorSuccess)

#define err_local_fail() \
  (Ne::Kernel::UserProcessScheduler::The().TheCurrentProcess().GetLocalCode() != Ne::Kernel::kErrorSuccess)

#define err_local_get() (Ne::Kernel::UserProcessScheduler::The().TheCurrentProcess().GetLocalCode())

#define err_global_ok() (Ne::Kernel::kErrorLocalNumber == Ne::Kernel::kErrorSuccess)
#define err_global_fail() (Ne::Kernel::kErrorLocalNumber != Ne::Kernel::kErrorSuccess)
#define err_global_get() (Ne::Kernel::kErrorLocalNumber)

namespace Ne::Kernel {
using ErrorT   = Int32;
using KPCError = ErrorT;

inline KPCError kErrorLocalNumber = 0UL;

inline constexpr KPCError kErrorSuccess            = 0;
inline constexpr KPCError kErrorExecutable         = 33;
inline constexpr KPCError kErrorExecutableLib      = 34;
inline constexpr KPCError kErrorFileNotFound       = 35;
inline constexpr KPCError kErrorDirectoryNotFound  = 36;
inline constexpr KPCError kErrorDiskReadOnly       = 37;
inline constexpr KPCError kErrorDiskIsFull         = 38;
inline constexpr KPCError kErrorProcessFault       = 39;
inline constexpr KPCError kErrorSocketHangUp       = 40;
inline constexpr KPCError kErrorThreadLocalStorage = 41;
inline constexpr KPCError kErrorMath               = 42;
inline constexpr KPCError kErrorNoNetwork          = 43;
inline constexpr KPCError kErrorHeapOutOfMemory    = 44;
inline constexpr KPCError kErrorNoSuchDisk         = 45;
inline constexpr KPCError kErrorFileExists         = 46;
inline constexpr KPCError kErrorFormatFailed       = 47;
inline constexpr KPCError kErrorNetworkTimeout     = 48;
inline constexpr KPCError kErrorInternal           = 49;
inline constexpr KPCError kErrorForkAlreadyExists  = 50;
inline constexpr KPCError kErrorOutOfTeamSlot      = 51;
inline constexpr KPCError kErrorHeapNotPresent     = 52;
inline constexpr KPCError kErrorNoEntrypoint       = 53;
inline constexpr KPCError kErrorDiskIsCorrupted    = 54;
inline constexpr KPCError kErrorDisk               = 55;
inline constexpr KPCError kErrorInvalidData        = 56;
inline constexpr KPCError kErrorAsync              = 57;
inline constexpr KPCError kErrorNonBlocking        = 58;
inline constexpr KPCError kErrorIPC                = 59;
inline constexpr KPCError kErrorSign               = 60;
inline constexpr KPCError kErrorInvalidCreds       = 61;
inline constexpr KPCError kErrorCDTrayBroken       = 62;
inline constexpr KPCError kErrorUnrecoverableDisk  = 63;
inline constexpr KPCError kErrorFileLocked         = 64;
inline constexpr KPCError kErrorDiskIsTooTiny      = 65;
inline constexpr KPCError kErrorDmaExhausted       = 66;
inline constexpr KPCError kErrorOutOfBitMapMemory  = 67;
inline constexpr KPCError kErrorTimeout            = 68;
inline constexpr KPCError kErrorAccessDenied       = 69;
inline constexpr KPCError kErrorUnavailable        = 70;
inline constexpr KPCError kErrorIPCTimeOut         = 71;
inline constexpr KPCError kErrorNetTimeOut         = 71;
/// Generic errors.
inline constexpr KPCError kErrorUnimplemented = -1;

#ifndef kKPCLastError
#define kKPCLastError kErrorNetTimeOut
#endif

/// @brief Does a system wide bug check.
/// @param void no params are needed.
/// @return if error-free: false, otherwise true.
Boolean err_bug_check_raise(Void);
}  // namespace Ne::Kernel

#endif
