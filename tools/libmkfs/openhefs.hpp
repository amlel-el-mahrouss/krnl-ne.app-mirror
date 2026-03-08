// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef LIBMKFS_OPENHEFS_H
#define LIBMKFS_OPENHEFS_H

#ifndef __GNUC__
#error !! libmkfs isn't working on MSVC yet !!
#endif

#include <cstdint>
#include <cstring>

#define kOpenHeFSVersion (0x0101)
#define kOpenHeFSMagic "OpenHeFS"
#define kOpenHeFSMagicLen (9U)

#define kOpenHeFSFileNameLen (256U)
#define kOpenHeFSPartNameLen (128U)

#define kOpenHeFSDefaultVolumeName u8"OpenHeFS Volume"

namespace mkfs::hefs {

// Drive kinds
enum {
  kOpenHeFSHardDrive         = 0xC0,  // Hard Drive
  kOpenHeFSSolidStateDrive   = 0xC1,  // Solid State Drive
  kOpenHeFSOpticalDrive      = 0x0C,  // Blu-Ray/DVD
  kOpenHeFSMassStorageDevice = 0xCC,  // USB
  kOpenHeFSScsiDrive         = 0xC4,  // SCSI Hard Drive
  kOpenHeFSFlashDrive        = 0xC6,
  kOpenHeFSUnknown           = 0xFF,  // Unknown device.
  kOpenHeFSDriveCount        = 8,
};

// Disk status
enum {
  kOpenHeFSStatusUnlocked = 0x18,
  kOpenHeFSStatusLocked,
  kOpenHeFSStatusError,
  kOpenHeFSStatusInvalid,
  kOpenHeFSStatusCount,
};

// Encodings
enum {
  kOpenHeFSEncodingFlagsUTF8 = 0x50,
  kOpenHeFSEncodingFlagsUTF16,
  kOpenHeFSEncodingFlagsUTF32,
  kOpenHeFSEncodingFlagsUTF16BE,
  kOpenHeFSEncodingFlagsUTF16LE,
  kOpenHeFSEncodingFlagsUTF32BE,
  kOpenHeFSEncodingFlagsUTF32LE,
  kOpenHeFSEncodingFlagsUTF8BE,
  kOpenHeFSEncodingFlagsUTF8LE,
  kOpenHeFSEncodingFlagsBinary,
  kOpenHeFSEncodingFlagsCount = 11,
  kOpenHeFSFlagsNone          = 0,
  kOpenHeFSFlagsReadOnly      = 0x100,
  kOpenHeFSFlagsHidden,
  kOpenHeFSFlagsSystem,
  kOpenHeFSFlagsArchive,
  kOpenHeFSFlagsDevice,
  kOpenHeFSFlagsCount = 7
};

// Time type
using ATime = std::uint64_t;

// File kinds
inline constexpr uint16_t kOpenHeFSFileKindRegular      = 0x00;
inline constexpr uint16_t kOpenHeFSFileKindDirectory    = 0x01;
inline constexpr uint16_t kOpenHeFSFileKindBlock        = 0x02;
inline constexpr uint16_t kOpenHeFSFileKindCharacter    = 0x03;
inline constexpr uint16_t kOpenHeFSFileKindFIFO         = 0x04;
inline constexpr uint16_t kOpenHeFSFileKindSocket       = 0x05;
inline constexpr uint16_t kOpenHeFSFileKindSymbolicLink = 0x06;
inline constexpr uint16_t kOpenHeFSFileKindUnknown      = 0x07;
inline constexpr uint16_t kOpenHeFSFileKindCount        = 0x08;

// Red-black tree colors
enum {
  kOpenHeFSInvalidColor = 0,
  kOpenHeFSRed          = 100,
  kOpenHeFSBlack,
  kOpenHeFSColorCount,
};

// Time constants
inline constexpr ATime kOpenHeFSTimeInvalid = 0x0000000000000000;
inline constexpr ATime kOpenHeFSTimeMax     = 0xFFFFFFFFFFFFFFFF - 1;

// Boot Node
struct __attribute__((packed)) BootNode {
  char          magic[kOpenHeFSMagicLen]{};
  char8_t       volumeName[kOpenHeFSPartNameLen]{};
  std::uint32_t version{};
  std::uint64_t badSectors{};
  std::uint64_t sectorCount{};
  std::uint64_t sectorSize{};
  std::uint32_t checksum{};
  std::uint8_t  diskKind{};
  std::uint8_t  encoding{};
  std::uint64_t startIND{};
  std::uint64_t endIND{};
  std::uint64_t indCount{};
  std::uint64_t diskSize{};
  std::uint16_t diskStatus{};
  std::uint16_t diskFlags{};
  std::uint16_t vid{};
  std::uint64_t startIN{};
  std::uint64_t endIN{};
  std::uint64_t startBlock{};
  std::uint64_t endBlock{};
  char          pad[272]{};
};

}  // namespace mkfs::hefs

#endif
