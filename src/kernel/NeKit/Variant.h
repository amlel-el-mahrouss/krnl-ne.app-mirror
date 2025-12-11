/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <NeKit/Config.h>
#include <NeKit/Json.h>
#include <NeKit/KString.h>
#include <NeKit/TOML.h>
#include <SwapKit/DiskSwap.h>

namespace Kernel {
class Variant final {
 public:
  enum struct VariantKind : Int {
    kInvalid = 0,
    kString  = 200,
    kBlob,
    kNull,
    kJson,
    kTOML,
    kSwap,
    kCount = kSwap - kString + 1,
  };

 public:
  explicit Variant() = delete;

 public:
  NE_COPY_DEFAULT(Variant)

  ~Variant() = default;

 public:
  template <typename CharKind>
  explicit Variant(KBasicString<CharKind>* stringView)
      : fPtr((VoidPtr) stringView), fKind(VariantKind::kString) {}

  explicit Variant(JsonObject<>* json) : fPtr((VoidPtr) json), fKind(VariantKind::kJson) {}

  explicit Variant(TOMLObject* toml) : fPtr((VoidPtr) toml), fKind(VariantKind::kTOML) {}

  explicit Variant(nullPtr ptr) : fPtr(ptr), fKind(VariantKind::kNull) {}

  explicit Variant(SWAP_DISK_HEADER* ptr) : fPtr(ptr), fKind(VariantKind::kSwap) {}

  explicit Variant(VoidPtr ptr) : fPtr(ptr), fKind(VariantKind::kBlob) {}

 public:
  const Char* ToString();

  /// ========================================================================
  /// @brief Returns the underlying pointer.
  /// @return the underlying pointer.
  /// ========================================================================
  VoidPtr Leak();

  template <typename T>
  T* As() {
    return reinterpret_cast<T*>(fPtr);
  }

  VariantKind& Kind();

 private:
  VoidPtr     fPtr{nullptr};
  VariantKind fKind{VariantKind::kNull};
};
}  // namespace Kernel
