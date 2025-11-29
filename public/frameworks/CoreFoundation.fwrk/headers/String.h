/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <CoreFoundation.fwrk/headers/Object.h>
#include <CoreFoundation.fwrk/headers/Ref.h>

namespace CF {
class CFString;
class CFStringBuilder;

/// ================================================================================
/// @brief CFString class for string manipulations.
/// ================================================================================
class CFString final CF_OBJECT {
 public:
  CFString() = delete;

  explicit CFString(const SizeT sz);
  explicit CFString(const Char* str);

  ~CFString();

  const char* asBytes() const;

  LIBSYS_COPY_DELETE(CFString);

 private:
  Char* mStr{nullptr};
};

/// ================================================================================
/// @brief CFStringBuilder class for string manipulations.
/// ================================================================================
class CFStringBuilder final {
 public:
  static CFRef<CFString> Construct(const Char*);
  static const Char*     FromBool(const Char*, BOOL);
  static const Char*     Format(const Char*, const Char*);

  static BOOL Equals(const Char, const Char*);
  static BOOL Equals(const Char*, const Char*);
};
}  // namespace CF