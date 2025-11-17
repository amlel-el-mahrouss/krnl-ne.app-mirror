/* -------------------------------------------

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

------------------------------------------- */

#pragma once

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Defines.h>
#include <NeKit/ErrorOr.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/Utils.h>

namespace Kernel {
inline auto kMinimumStringSize = 8196;

/// @brief Kernel string class, not dynamic.
template <typename CharKind = Char>
class KBasicString final {
 public:
  explicit KBasicString() {
    fDataSz = kMinimumStringSize;

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

  bool operator==(const KBasicString<CharKind>& rhs) const;
  bool operator!=(const KBasicString<CharKind>& rhs) const;

  KBasicString<CharKind>& operator+=(const CharKind* rhs);
  KBasicString<CharKind>& operator+=(const KBasicString<CharKind>& rhs);

  operator const char*() { return fData; }

  operator bool() { return fData; }

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
  template <typename CharKind = Char>
  static ErrorOr<KBasicString<CharKind>> Construct(const CharKind* data);
  template <typename CharKind = Char>
  static const CharKind* FromBool(const CharKind* fmt, bool n);
  template <typename CharKind = Char>
  static const CharKind* Format(const CharKind* fmt, const CharKind* from);
  template <typename CharKind = Char>
  static bool Equals(const CharKind* lhs, const CharKind* rhs);
};
}  // namespace Kernel

#include <NeKit/KString.inl>
