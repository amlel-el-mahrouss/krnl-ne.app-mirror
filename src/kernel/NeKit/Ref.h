
/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef _NEKIT_REF_H_
#define _NEKIT_REF_H_

#include <CompilerKit/CompilerKit.h>
#include <KernelKit/HeapMgr.h>
#include <NeKit/Config.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/Vettable.h>

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
  using Type = T;

  Ref(Type* cls) : fClass(*cls) {}
  Ref(Type cls) : fClass(cls) {}

  Ref& operator=(Type ref) {
    fClass = ref;
    return *this;
  }

  NE_COPY_DEFAULT(Ref)

 public:
  Type operator->() const { return fClass; }

  Type& Leak() { return fClass; }

  Type& TryLeak() { return fClass; }

  Type operator*() { return fClass; }

  operator bool() { return Vettable<Type>::kValue; }
  bool operator!() { return !Vettable<Type>::kValue; }

 private:
  Type fClass;
};

template <typename T>
class NonNullRef final {
 public:
  using RefType = Ref<T>;
  using Type    = T;

  NonNullRef()        = delete;
  NonNullRef(nullPtr) = delete;

  NonNullRef(Type* ref) : fRef(ref) { MUST_PASS(ref); }
  NonNullRef(RefType ref) : fRef(ref) { MUST_PASS(ref); }

  Ref<T>& operator->() {
    MUST_PASS(fRef);
    return fRef;
  }

  NonNullRef& operator=(const NonNullRef<T>& ref) = delete;
  NonNullRef(const NonNullRef<T>& ref)            = delete;

 private:
  Ref<T> fRef{};
};

using RefAny        = Ref<Any>;
using NonNullRefAny = NonNullRef<Any>;
}  // namespace Kernel

#endif  // ifndef _NEKIT_REF_H_
