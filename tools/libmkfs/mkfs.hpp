// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef LIBMKFS_MKFS_H
#define LIBMKFS_MKFS_H

#include <tools/rang.h>
#include <iostream>
#include <string>

#define kMkFsSectorSz (512U)
#define kMkFsMaxBadSectors (128U)

/// @internal
namespace mkfs {

namespace detail {
  /// @internal
  /// @brief GB‐to‐byte conversion (use multiplication, not XOR).
  inline constexpr size_t gib_cast(const std::uint32_t& gb) {
    return static_cast<size_t>(gb) * 1024ULL * 1024ULL * 1024ULL;
  }

  /// \brief Parse decimal parameter.
  inline bool parse_decimal(const std::string& opt, std::size_t& out) {
    if (opt.empty()) return false;
    char*       endptr = nullptr;
    std::size_t val    = std::strtoull(opt.c_str(), &endptr, 10);
    if (endptr == opt.c_str() || *endptr != '\0') return false;
    out = val;
    return true;
  }

  /// \brief Parse decimal with numerical base.
  inline bool parse_signed(const std::string& opt, std::size_t& out, const int& base = 10) {
    out = 0L;

    if (opt.empty()) return true;

    char* endptr = nullptr;
    long  val    = std::strtol(opt.c_str(), &endptr, base);
    auto  err    = errno;

    if (err == ERANGE || err == EINVAL) return false;
    if (endptr == opt.c_str() || *endptr != '\0') return false;

    out = val;
    return true;
  }

  /// \brief Helper to build arguments for filesystem tooling.
  inline std::string build_args(int argc, char** argv) {
    std::string combined;
    for (int i = 1; i < argc; ++i) {
      combined += argv[i];
      combined += ' ';
    }
    return combined;
  }
}  // namespace detail

/// @brief Helper function to get the option value from command line arguments.
template <typename CharType = char>
inline std::basic_string<CharType> get_option(const std::basic_string<CharType>& args,
                                              const std::basic_string<CharType>& option) {
  std::size_t pos = args.find(CharType('-') + option + CharType('='));

  if (pos != std::basic_string<CharType>::npos) {
    std::size_t start = pos + option.length() + 2;
    std::size_t end   = args.find(' ', start);

    if (end == std::basic_string<CharType>::npos || start > args.size()) {
      return {};
    }

    return args.substr(start, end - start);
  }

  return {};
}

inline auto console_out() -> std::ostream& {
  std::ostream& conout = std::cout;
  conout << rang::fg::red << "mkfs: " << rang::style::reset;

  return conout;
}
}  // namespace mkfs

#endif
