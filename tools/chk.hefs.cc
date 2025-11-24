/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#include <tools/libmkfs/mkfs.h>
#include <tools/libmkfs/openhefs.h>
#include <cstdlib>
#include <fstream>

/// @note decimal base.
static uint16_t kNumericalBase = 10;

int main(int argc, char** argv) {
  if (argc < 2) {
    mkfs::console_out() << "fsck: hefs: usage: chk.hefs -in=<input_device> -b=<origin>"
                        << "\n";
    return EXIT_FAILURE;
  }

  auto args = mkfs::detail::build_args(argc, argv);

  auto opt_disk = mkfs::get_option<char>(args, "in");

  auto origin = mkfs::get_option<char>(args, "b");

  if (opt_disk.empty()) {
    mkfs::console_out() << "fsck: hefs: error: OpenHeFS partition is empty! Exiting..."
                        << "\n";
    return EXIT_FAILURE;
  }

  auto out_origin = 0L;

  if (!mkfs::detail::parse_signed(origin, out_origin, kNumericalBase)) {
    mkfs::console_out() << "hefs: error: Invalid -org=<dec> argument.\n";
    return EXIT_FAILURE;
  }

  std::ifstream output_device(opt_disk, std::ios::binary);

  if (!output_device.good()) {
    mkfs::console_out() << "hefs: error: Unable to open output device: " << opt_disk << "\n";
    return EXIT_FAILURE;
  }

  output_device.seekg(out_origin);

  if (!output_device.good()) {
    mkfs::console_out() << "hefs: error: Failed seek to origin.\n";
    return EXIT_FAILURE;
  }

  // @note use modern intializer list here to empty out the fields according to the struct layout.
  mkfs::hefs::BootNode boot_node{};

  output_device.read(reinterpret_cast<char*>(&boot_node), sizeof(boot_node));

  if (strncmp(boot_node.magic, kOpenHeFSMagic, kOpenHeFSMagicLen) != 0 || boot_node.sectorCount < 1 ||
      boot_node.sectorSize < kMkFsSectorSz) {
    mkfs::console_out() << "hefs: error: Device is not an OpenHeFS disk: " << opt_disk << "\n";
    return EXIT_FAILURE;
  }

  if (boot_node.badSectors > kMkFsMaxBadSectors) {
    mkfs::console_out() << "hefs: error: OpenHeFS disk has too much bad sectors: " << opt_disk
                        << "\n";
    return EXIT_FAILURE;
  }

  mkfs::console_out() << "hefs: OpenHeFS partition is healthy. Exiting...\n";

  output_device.close();

  return EXIT_SUCCESS;
}
