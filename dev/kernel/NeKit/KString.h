/* -------------------------------------------

  Copyright (C) 2024-2025, Amlal El Mahrouss, all rights reserved.

------------------------------------------- */

#pragma once

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Defines.h>
#include <NeKit/ErrorOr.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/Utils.h>

#define kMinimumStringSize (8196U)

namespace Kernel {
/// @brief Kernel string class, not dynamic.
template <typename CharKind = Char>
class BasicKString final {
 public:
  explicit BasicKString() {
    fDataSz = kMinimumStringSize;

    fData = new CharKind[fDataSz];
    MUST_PASS(fData);

    rt_set_memory(fData, 0, fDataSz);
  }

  explicit BasicKString(SizeT Sz) : fDataSz(Sz) {
    MUST_PASS(Sz > 1);

    fData = new CharKind[Sz];
    MUST_PASS(fData);

    rt_set_memory(fData, 0, Sz);
  }

  ~BasicKString() {
    if (fData) {
      delete[] fData;
      fData = nullptr;
    }
  }

  NE_COPY_DEFAULT(BasicKString)

  CharKind*       Data();
  const CharKind* CData() const;
  Size            Length() const;

  bool operator==(const CharKind* rhs) const;
  bool operator!=(const CharKind* rhs) const;

  bool operator==(const BasicKString<CharKind>& rhs) const;
  bool operator!=(const BasicKString<CharKind>& rhs) const;

  BasicKString<CharKind>& operator+=(const CharKind* rhs);
  BasicKString<CharKind>& operator+=(const BasicKString<CharKind>& rhs);

  operator const char*() { return fData; }

  operator bool() { return fData; }

  bool operator!() { return fData; }

 private:
  CharKind* fData{nullptr};
  Size      fDataSz{0};
  Size      fCur{0};

  friend class KStringBuilder;
};

using KString   = BasicKString<>;
using KStringOr = ErrorOr<KString>;

class KStringBuilder final {
 public:
  template <typename CharKind = Char>
  static ErrorOr<BasicKString<CharKind>> Construct(const CharKind* data);
  template <typename CharKind = Char>
  static const CharKind* FromBool(const CharKind* fmt, bool n);
  template <typename CharKind = Char>
  static const CharKind* Format(const CharKind* fmt, const CharKind* from);
  template <typename CharKind = Char>
  static bool Equals(const CharKind* lhs, const CharKind* rhs);
};
}  // namespace Kernel

#include <NeKit/KString.inl>
