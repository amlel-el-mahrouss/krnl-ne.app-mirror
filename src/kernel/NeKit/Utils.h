// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef NEKIT_UTILS_H
#define NEKIT_UTILS_H

#include <NeKit/Config.h>

namespace Kernel {
/// =========================================================== ///
/// @brief ASCII API
/// =========================================================== ///

Int         rt_copy_memory(const voidPtr src, voidPtr dst, Size len);
Int         rt_move_memory(const voidPtr src, voidPtr dst, Size len);
voidPtr     rt_set_memory(voidPtr dst, UInt32 val, Size len);
void        rt_zero_memory(voidPtr pointer, Size len);
Int         rt_string_cmp(const Char* src, const Char* cmp, Size len);
const Char* rt_alloc_string(const Char* text);
Size        rt_string_len(const Char* str);
Size        rt_string_len(const Char* str, SizeT _len);
Boolean     rt_to_string(Char* str_out, UInt64 base, Int32 limit);
Boolean     rt_is_newln(Int chr);
Boolean     rt_is_space(Int chr);
Int32       rt_is_alnum(Int character);
Int         rt_to_uppercase(Int c);
Int         rt_to_lower(Int c);
voidPtr     rt_string_in_string(const Char* in, const Char* needle);
char*       rt_string_has_char(Char* str, Char chr);

/// =========================================================== ///
/// @brief Safe memory functions API
/// =========================================================== ///

Int     rt_copy_memory_safe(const voidPtr src, voidPtr dst, Size len, Size dst_size);
voidPtr rt_set_memory_safe(voidPtr dst, UInt32 value, Size len, Size dst_size);

/// =========================================================== ///
/// @brief UNICODE API
/// =========================================================== ///

Int  urt_string_cmp(const Utf8Char* src, const Utf8Char* cmp, Size len);
Void urt_set_memory(const voidPtr src, UInt32 dst, Size len);
Int  urt_copy_memory(const voidPtr src, voidPtr dst, Size len);
Size urt_string_len(const Utf8Char* str);

/// =========================================================== ///
/// @brief OpenEncoding API
/// =========================================================== ///

template <typename CharType = Char>
inline constexpr SizeT oe_string_len(const CharType* str) {
  if (!str) return 0;

#if __cplusplus == 202302L
  if
    consteval { return ARRAY_SIZE(str); }
  else {
    SizeT len{0};
    while (str[len] != 0) ++len;
    return len;
  }
#else
  SizeT len{0};

  while (str[len] != 0) ++len;
  return len;
#endif
}
}  // namespace Kernel

#endif
