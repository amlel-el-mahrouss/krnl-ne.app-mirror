// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <cstdlib>
#include <fstream>
#include <tools/libmkfs/mkfs.hpp>
#include <tools/libmkfs/openhefs.hpp>

/// @note decimal base.
static uint16_t kNumericalBase = 10;
static auto     kMinArgs       = 2;

int main(int argc, char** argv) {
  if (argc < kMinArgs) {
    mkfs::console_out() << "chk: hefs: usage: chk.hefs -in=<input_device> -b=<origin>"
                        << "\n";
    return EXIT_FAILURE;
  }

  auto args     = mkfs::detail::build_args(argc, argv);
  auto opt_disk = mkfs::get_option<char>(args, "in");
  auto origin   = mkfs::get_option<char>(args, "b");

  if (opt_disk.empty()) {
    mkfs::console_out() << "chk: hefs: error: OpenHeFS partition is empty! Exiting..."
                        << "\n";
    return EXIT_FAILURE;
  }

  auto out_origin = 0UL;

  if (!mkfs::detail::parse_signed(origin, out_origin, kNumericalBase)) {
    mkfs::console_out() << "chk: hefs: error: Invalid -org=<dec> argument.\n";
    return EXIT_FAILURE;
  }

  std::ifstream output_device(opt_disk, std::ios::binary);

  if (!output_device.good()) {
    mkfs::console_out() << "chk: hefs: error: Unable to open output device: " << opt_disk << "\n";
    return EXIT_FAILURE;
  }

  output_device.seekg(out_origin);

  if (!output_device.good()) {
    mkfs::console_out() << "chk: hefs: error: Failed seek to origin.\n";
    return EXIT_FAILURE;
  }

  // @note use modern intializer list here to empty out the fields according to the struct layout.
  mkfs::hefs::BootNode boot_node{};

  output_device.read(reinterpret_cast<char*>(&boot_node), sizeof(boot_node));

  if (strncmp(boot_node.magic, kOpenHeFSMagic, kOpenHeFSMagicLen) != 0 ||
      boot_node.sectorCount < 1 || boot_node.sectorSize < kMkFsSectorSz) {
    mkfs::console_out() << "chk: hefs: error: Device does not contain an OpenHeFS disk: "
                        << opt_disk << "\n";
    return EXIT_FAILURE;
  }

  if (boot_node.badSectors > kMkFsMaxBadSectors) {
    mkfs::console_out() << "chk: hefs: error: OpenHeFS disk has too much bad sectors: " << opt_disk
                        << "\n";
    return EXIT_FAILURE;
  }

  mkfs::console_out() << "chk: hefs: OpenHeFS partition is healthy. Exiting...\n";

  return EXIT_SUCCESS;
}
