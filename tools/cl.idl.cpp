// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/ne-kernel

#include <cstdlib>
#include <fstream>
#include <tools/libidl/idl.hpp>

static uint16_t kNumericalBase = 10;
static auto     kMinArgs       = 4;

int main(int argc, char** argv) {
  if (argc < kMinArgs) {
    std::cout << "cl.idl: invalid argument count, a minimum of 3 is required.\n";
    return EXIT_FAILURE;
  }

  std::ofstream out_fp(std::string{argv[1]} + LIBIDL_FILE_EXT);

  for (int i = {2}; i < argc; i += 2) {
    const char* key   = argv[i];
    const char* value = argv[i + 1];

    if (::strlen(value) > LIBIDL_MAX_LAYOUT_LEN) break;
    if (::strlen(key) > LIBIDL_MAX_LAYOUT_LEN) break;

    LIBIDL_DECL(key, value, out_fp);
  }

  return EXIT_SUCCESS;
}