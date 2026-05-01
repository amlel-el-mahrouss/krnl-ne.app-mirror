// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/ne-kernel

#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define LIBIDL_MAGIC_NUMBER ("IDLBUF")
#define LIBIDL_MAX_LAYOUT_LEN (32)
#define LIBIDL_FILE_EXT ".idl"

#define LIBIDL_DECL(KEY, VALUE, FP)                                           \
  {                                                                           \
    ::libidl::detail::libidl_interface<decltype(KEY), decltype(VALUE)> idl;   \
    ::memset(&idl, 0, sizeof(idl));                                           \
    idl.key_.layout_ = 0;                                                     \
    ::memcpy(idl.key_.val_, KEY, ::strlen(KEY));                              \
    idl.value_.layout_ = ::strlen(KEY) + LIBIDL_MAX_LAYOUT_LEN;                 \
    ::memcpy(idl.value_.val_, VALUE, ::strlen(VALUE));                        \
    ::memcpy(idl.magic_, LIBIDL_MAGIC_NUMBER, ::strlen(LIBIDL_MAGIC_NUMBER)); \
    FP.write((char*) &idl, sizeof(idl));                                      \
    FP.flush();                                                               \
  }

namespace libidl {

namespace detail {

  template <typename Key, class Value>
  struct libidl_interface;

  template <typename Value>
  struct libidl_wrapper;

  template <typename Key, class Value>
  struct libidl_interface {
    using magic_type = char[6];
    magic_type            magic_;
    libidl_wrapper<Key>   key_;
    libidl_wrapper<Value> value_;
  };

  template <typename Value>
  struct libidl_wrapper {
    using type = char[LIBIDL_MAX_LAYOUT_LEN];
    int  layout_;
    type val_;
  };

}  // namespace detail

}  // namespace libidl