// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

//! @file ThreadLocalStorage.inl
//! @brief Allocate resources from the process's heap storage.

#ifndef KERNELKIT_USERPROCESSSCHEDULER_H
#include <KernelKit/ProcessScheduler.h>
#endif

namespace Kernel {

template <class T>
inline T* tls_new_ptr(void) {
  using namespace Kernel;

  auto ref_process = UserProcessScheduler::The().TheCurrentProcess();

  auto pointer = ref_process.New(sizeof(T));

  if (pointer.Error()) return nullptr;

  return reinterpret_cast<T*>(pointer.Leak().Leak());
}

//! @brief Delete process pointer.
//! @param obj The pointer to delete.
template <class T>
inline Kernel::Bool tls_delete_ptr(T* obj) {
  using namespace Kernel;

  if (!obj) return No;

  auto ref_process = UserProcessScheduler::The().TheCurrentProcess();

  ErrorOr<T*> obj_wrapped{obj};

  return ref_process.Delete(obj_wrapped);
}

//! @brief Delete process pointer.
//! @param obj The pointer to delete.
template <class T>
inline Kernel::Bool tls_delete_ptr(Kernel::ErrorOr<T> obj) {
  return tls_delete_ptr(obj.Leak());
}

//! @brief Delete process pointer.
//! @param obj The pointer to delete.
template <class T>
inline Kernel::Bool tls_delete_ptr(Kernel::ErrorOr<T*> obj) {
  return tls_delete_ptr(obj->Leak());
}

/// @brief Allocate a C++ class, and then call the constructor of it.
/// @tparam T class type.
/// @tparam ...Args varg class type.
/// @param args arguments list.
/// @return Class instance.
template <class T, class... Args>
T* tls_new_class(Args&&... args) {
  using namespace Kernel;

  T* obj = tls_new_ptr<T>();

  if (obj) {
    *obj = T(forward(args)...);
    return obj;
  }

  return nullptr;
}

/// @brief Delete a C++ class (call constructor first.)
/// @tparam T
/// @param obj
/// @return
template <class T>
inline Kernel::Bool tls_delete_class(T* obj) {
  using namespace Kernel;

  if (!obj) return No;

  obj->~T();
  return tls_delete_ptr(obj);
}

}  // namespace Kernel