// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef NEKIT_VARIANT_H
#define NEKIT_VARIANT_H

#include <NeKit/Config.h>
#include <NeKit/Json.h>
#include <NeKit/KString.h>
#include <NeKit/TOML.h>
#include <SwapKit/DiskSwap.h>

namespace Ne::Kernel {
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
  explicit Variant(SwapDiskHdr* ptr) : fPtr(ptr), fKind(VariantKind::kSwap) {}
  explicit Variant(VoidPtr ptr) : fPtr(ptr), fKind(VariantKind::kBlob) {}

 public:
  KString ToString();

  /// ========================================================================
  /// @brief Returns the underlying pointer.
  /// @return the underlying pointer as a reference.
  /// ========================================================================
  RefAny Leak();

  template <typename T>
  T* As();

  VariantKind& Kind();

 private:
  VoidPtr     fPtr{nullptr};
  VariantKind fKind{VariantKind::kNull};
};
}  // namespace Ne::Kernel

#include "Variant.inl"

#endif
