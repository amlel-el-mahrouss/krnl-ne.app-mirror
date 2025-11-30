/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <libSystem/SystemKit/System.h>

namespace CF {
template <typename Key, typename Value>
class CFDictionary final {
 public:
  explicit CFDictionary() = default;
  ~CFDictionary()         = default;

  CFDictionary& operator=(const CFDictionary&) = default;
  CFDictionary(const CFDictionary&)            = default;

  Value& operator[](Key& at) {
    return this->Find(at);
  }

  Bool Empty() { return this->fCount > 0; }

  Bool Find(Key& key) { NE_UNUSED(key); return false; }

  operator bool() { return !this->Empty(); }

 private:
  SizeT fCount{0UL};
};

template <typename KeyType, typename ValueType>
inline auto make_dict() {
  return CFDictionary<KeyType, ValueType>{};
}
}  // namespace CF
