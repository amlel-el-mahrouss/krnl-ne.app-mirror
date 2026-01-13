// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef __KERNEL_KIT_TLS_H__
#define __KERNEL_KIT_TLS_H__

#include <NeKit/Config.h>
#include <NeKit/ErrorOr.h>

///! @brief Thread Local Storage for NeKernel.

#define kCookieMag0Idx (0U)
#define kCookieMag1Idx (1U)
#define kCookieMag2Idx (2U)

#define kCookieMag0 'N'
#define kCookieMag1 'K'
#define kCookieMag2 'O'

#define kCookieMagLen (3U)

struct THREAD_INFORMATION_BLOCK;

/// @brief Thread Information Block.
/// Located in GS on AMD64, other architectures have their own stuff. (64x0, 32x0, ARM64)
struct PACKED THREAD_INFORMATION_BLOCK final {
  Kernel::Char    Cookie[kCookieMagLen]{0};  //! Thread Magic Number.
  Kernel::VoidPtr UserData{nullptr};  //! Thread Information Record (User defined canary structure)
};

///! @brief Cookie Sanity check.
Kernel::Boolean tls_check_tib(THREAD_INFORMATION_BLOCK* the_tib);

///! @brief new ptr syscall.
template <typename T>
T* tls_new_ptr(void);

///! @brief delete ptr syscall.
template <typename T>
Kernel::Boolean tls_delete_ptr(T* ptr);

//! @brief Delete process pointer.
//! @param obj The pointer to delete.
template <typename T>
inline Kernel::Bool tls_delete_ptr(Kernel::ErrorOr<T> obj);

//! @brief Delete process pointer.
//! @param obj The pointer to delete.
template <typename T>
inline Kernel::Bool tls_delete_ptr(Kernel::ErrorOr<T*> obj);

template <typename T, typename... Args>
T* tls_new_class(Args&&... args);

/// @brief TLS install TIB and PIB. (syscall)
EXTERN_C Kernel::Void rt_install_tib(THREAD_INFORMATION_BLOCK* TIB, THREAD_INFORMATION_BLOCK* PIB);

/// @brief TLS check (syscall)
EXTERN_C Kernel::Bool tls_check_syscall_impl(Kernel::VoidPtr TIB);

#include "ThreadLocalStorage.inl"

// last rev 7/7/24

#endif /* ifndef __KERNEL_KIT_TLS_H__ */
