
/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef _NEKIT_REF_H_
#define _NEKIT_REF_H_

#include <CompilerKit/CompilerKit.h>
#include <KernelKit/HeapMgr.h>
#include <NeKit/Config.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/Vet.h>

namespace Kernel {
/// =========================================================== ///
/// @brief Reference wrapper class.                          ///
/// =========================================================== ///
template <typename T>
class Ref final {
 public:
  explicit Ref() = default;
  ~Ref()         = default;

 public:
  Ref(T* cls) : fClass(*cls) {}
  Ref(T cls) : fClass(cls) {}

  Ref& operator=(T ref) {
    fClass = ref;
    return *this;
  }

  NE_COPY_DEFAULT(Ref)

 public:
  T operator->() const { return fClass; }

  T& Leak() { return fClass; }

  T& TryLeak() { return fClass; }

  T operator*() { return fClass; }

  operator bool() { return Vettable<T>::kValue; }
  bool operator!() { return !Vettable<T>::kValue; }

 private:
  T fClass;
};

template <typename T>
class NonNullRef final {
 public:
  NonNullRef()        = delete;
  NonNullRef(nullPtr) = delete;

  NonNullRef(T* ref) : fRef(ref) { MUST_PASS(ref); }
  NonNullRef(Ref<T> ref) : fRef(ref) { MUST_PASS(ref); }

  Ref<T>& operator->() {
    MUST_PASS(fRef);
    return fRef;
  }

  NonNullRef& operator=(const NonNullRef<T>& ref) = delete;
  NonNullRef(const NonNullRef<T>& ref)            = default;

 private:
  Ref<T> fRef{};
};

using RefAny        = Ref<Any>;
using NonNullRefAny = NonNullRef<Any>;
}  // namespace Kernel

#endif  // ifndef _NEKIT_REF_H_
