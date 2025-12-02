/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Config.h>
#include <NeKit/ErrorOr.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/Utils.h>

namespace Kernel {
inline constexpr Int kMinimumStringSize = 8196;

/// @brief Kernel string class, not dynamic.
template <typename CharKind = Char, Int MinSz = kMinimumStringSize>
class KBasicString final {
 public:
  explicit KBasicString() {
    fDataSz = MinSz;

    fData = new CharKind[fDataSz];
    MUST_PASS(fData);

    rt_set_memory(fData, 0, fDataSz);
  }

  explicit KBasicString(SizeT Sz) : fDataSz(Sz) {
    MUST_PASS(Sz > 1);

    fData = new CharKind[Sz];
    MUST_PASS(fData);

    rt_set_memory(fData, 0, Sz);
  }

  ~KBasicString() {
    if (fData) {
      delete[] fData;
      fData = nullptr;
    }
  }

  NE_COPY_DEFAULT(KBasicString)

  CharKind*       Data();
  const CharKind* CData() const;
  Size            Length() const;

  bool operator==(const CharKind* rhs) const;
  bool operator!=(const CharKind* rhs) const;

  bool operator==(const KBasicString<CharKind, MinSz>& rhs) const;
  bool operator!=(const KBasicString<CharKind, MinSz>& rhs) const;

  KBasicString<CharKind, MinSz>& operator+=(const CharKind* rhs);
  KBasicString<CharKind, MinSz>& operator+=(const KBasicString<CharKind, MinSz>& rhs);

  operator const char*() { return fData; }

  explicit operator bool() { return fData; }

  bool operator!() { return fData; }

 private:
  CharKind* fData{nullptr};
  Size      fDataSz{0};
  Size      fCur{0};

  friend class KStringBuilder;
};

using KString   = KBasicString<>;
using KStringOr = ErrorOr<KString>;

class KStringBuilder final {
 public:
  template <typename CharKind = Char, Int MinSz = kMinimumStringSize>
  static ErrorOr<KBasicString<CharKind, MinSz>> Construct(const CharKind* data);
  template <typename CharKind = Char>
  static const CharKind* FromBool(const CharKind* fmt, bool n);
  template <typename CharKind = Char>
  static const CharKind* Format(const CharKind* fmt, const CharKind* from);
  template <typename CharKind = Char>
  static bool Equals(const CharKind* lhs, const CharKind* rhs);
};
}  // namespace Kernel

#include <NeKit/KString.inl>
