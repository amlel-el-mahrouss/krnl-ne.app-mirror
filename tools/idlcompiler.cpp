// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/ne-kernel

#include <cstdlib>
#include <fstream>
#include <tools/libmkfs/mkfs.hpp>

static uint16_t kNumericalBase = 10;
static auto     kMinArgs       = 2;

int main(int argc, char** argv) {
  if (argc < kMinArgs) {
    mkfs::console_out() << "idlcompiler: usage: idlcompiler -in=<file_path> -specs=<specification_path>"
                        << "\n";
    mkfs::console_out() << "idlcompiler: note: this tool is compatible with ANT as well.\n";

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}