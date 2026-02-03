// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef FSKIT_OPENHEFS_H
#define FSKIT_OPENHEFS_H

#include <CompilerKit/CompilerKit.h>
#include <KernelKit/DriveMgr.h>
#include <KernelKit/UserMgr+User.h>
#include <NeKit/Config.h>
#include <NeKit/Crc32.h>
#include <NeKit/KString.h>
#include <hint/CompilerHint.h>

/// @file OpenHeFS.h
/// @brief OpenHeFS filesystem support.

#define kOpenHeFSVersion (0x0104)
#define kOpenHeFSMagic "OpenHeFS"
#define kOpenHeFSMagicLen (9U)

#define kOpenHeFSBlockLen (512U)
#define kOpenHeFSFileNameLen (256U)
#define kOpenHeFSPartNameLen (128U)

#define kOpenHeFSMinimumDiskSize (gib_cast(128))

#define kOpenHeFSDefaultVolumeName u8"OpenHeFS Volume"

#define kOpenHeFSINDStartOffset (sizeof(HEFS_BOOT_NODE))
#define kOpenHeFSINStartOffset (sizeof(HEFS_INDEX_NODE_DIRECTORY))

#define kOpenHeFSRootDirectory "/"
#define kOpenHeFSRootDirectoryU8 u8"/"

#define kOpenHeFSSeparator '/'
#define kOpenHeFSUpDir ".."

#define kOpenHeFSRootDirectoryLen (2U)

#define kOpenHeFSSearchAllStr u8"*"

struct HEFS_BOOT_NODE;
struct HEFS_INDEX_NODE;
struct HEFS_INDEX_NODE_DIRECTORY;
struct HEFS_JOURNAL_NODE;

enum : UInt8 {
  kOpenHeFSHardDrive         = 0xC0,  // Hard Drive
  kOpenHeFSSolidStateDrive   = 0xC1,  // Solid State Drive
  kOpenHeFSOpticalDrive      = 0x0C,  // Blu-Ray/DVD
  kOpenHeFSMassStorageDevice = 0xCC,  // USB
  kOpenHeFSScsiDrive         = 0xC4,  // SCSI Hard Drive
  kOpenHeFSFlashDrive        = 0xC6,
  kOpenHeFSUnknown           = 0xFF,  // Unknown device.
  kOpenHeFSDriveCount        = 8,
};

enum : UInt8 {
  kOpenHeFSStatusUnlocked = 0x18,
  kOpenHeFSStatusLocked,
  kOpenHeFSStatusError,
  kOpenHeFSStatusInvalid,
  kOpenHeFSStatusCount,
};

enum : UInt16 {
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

inline constexpr UInt16 kOpenHeFSFileKindRegular      = 0x00;
inline constexpr UInt16 kOpenHeFSFileKindDirectory    = 0x01;
inline constexpr UInt16 kOpenHeFSFileKindBlock        = 0x02;
inline constexpr UInt16 kOpenHeFSFileKindCharacter    = 0x03;
inline constexpr UInt16 kOpenHeFSFileKindFIFO         = 0x04;
inline constexpr UInt16 kOpenHeFSFileKindSocket       = 0x05;
inline constexpr UInt16 kOpenHeFSFileKindSymbolicLink = 0x06;
inline constexpr UInt16 kOpenHeFSFileKindUnknown      = 0x07;
inline constexpr UInt16 kOpenHeFSFileKindCount        = 0x08;

/// @brief OpenHeFS blocks are array containing sparse blocks of data.
/// @details The blocks are used to store the data of a file. Each block is a pointer to a block of
/// data on the disk.
inline constexpr UInt16 kOpenHeFSSliceCount = 0x10;

inline constexpr UInt16 kOpenHeFSInvalidVID = 0xFFFF;

namespace Kernel {
/// @brief Access time type.
/// @details Used to keep track of the INode, INodeDir allocation status.
typedef UInt64 ATime;

/// @brief OpenHeFS Boot node.
/// @details Acts like a superblock, it contains the information about the filesystem.
/// @note The boot node is the first block of the filesystem.
struct PACKED HEFS_BOOT_NODE final {
  Char     fMagic[kOpenHeFSMagicLen];       /// @brief Magic number of the filesystem.
  Utf8Char fVolName[kOpenHeFSPartNameLen];  /// @brief Volume name.
  UInt32   fVersion;                        /// @brief Version of the filesystem.
  UInt64   fBadSectors;                     /// @brief Number of bad sectors in the filesystem.
  UInt64   fSectorCount;                    /// @brief Number of sectors in the filesystem.
  UInt64   fSectorSize;                     /// @brief Size of the sector.
  UInt32   fChecksum;                       /// @brief Checksum of the boot node.
  UInt8    fDiskKind;  /// @brief Kind of the drive. (Hard Drive, Solid State Drive, Optical
                       /// Drive, etc).
  UInt8  fEncoding;    /// @brief Encoding of the filesystem. (UTF-8, UTF-16, etc).
  UInt64 fStartIND;    /// @brief Start of the INode directory tree.
  UInt64 fEndIND;      /// @brief End of the INode directory tree.
  UInt64 fINDCount;    /// @brief Number of leafs in the INode tree.
  UInt64 fDiskSize;    /// @brief Size of the disk. (Could be a virtual size, that is not the
                       /// real size of the disk.)
  UInt16 fDiskStatus;  /// @brief Status of the disk. (locked, unlocked, error, invalid).
  UInt16 fDiskFlags;   /// @brief Flags of the disk. (read-only, read-write, etc).
  UInt16 fVID;  /// @brief Virtual Identification Number within an EPM disk. (0xFFFF if not used).
  UInt64 fStartIN;     /// @brief Start INodes range
  UInt64 fEndIN;       /// @brief End INodes range
  UInt64 fStartBlock;  /// @brief Start Blocks range
  UInt64 fEndBlock;    /// @brief End Blocks range
  UInt64 fJournalLBA;  /// @brief Boot Node's COW journal LBA.
  Char   fPad[264];
};

inline constexpr ATime kOpenHeFSTimeInvalid = 0x0000000000000000;
inline constexpr ATime kOpenHeFSTimeMax     = 0xFFFFFFFFFFFFFFFF - 1;

/// @brief Journal Node structure
/// @param fHashPath target hash path
/// @param fStatus target status
/// @param fCopyElem copy of element
/// @param fCopyKind kind of element
struct PACKED HEFS_JOURNAL_NODE {
  UInt64 fHashPath;
  UInt64 fStatus;
  UInt64 fCopyElem;
  UInt8  fCopyKind;
  UInt8  fPad[487];
};

/// @brief This enum defines the opcode of the journal, here mentioned as 'kinds'
enum HeFSJournalKind : UInt8 {
  kJournalKindInvalid  = 0x00,
  kJournalKindWrite    = 0x01,
  kJournalKindRename   = 0x02,
  kJournalKindDelete   = 0x03,
  kJournalKindFlagEdit = 0x04,
  kJournalKindCreate   = 0x05,
  kJournalKindCount,
};

/// @brief OpenHeFS index node.
/// @details This structure is used to store the file information of a file.
/// @note The index node is a special type of INode that contains the file information.
/// @note The index node is used to store the file information of a file.
struct PACKED HEFS_INDEX_NODE final {
  UInt64 fHashPath;  /// @brief File name.
  UInt32 fFlags;     /// @brief File flags.
  UInt16 fKind;      /// @brief File kind. (Regular, Directory, Block, Character, FIFO, Socket,
                     /// Symbolic Link, Unknown).
  UInt32 fSize;      /// @brief File size.
  UInt32 fChecksum;  /// @brief Checksum.

  Boolean fSymLink;  /// @brief Is this a symbolic link? (if yes, the fName is the path to
                     /// the file and blocklinkstart and end contains it's inodes.)

  ATime  fCreated, fAccessed, fModified, fDeleted;  /// @brief File timestamps.
  UInt32 fUID, fGID;                                /// @brief User ID and Group ID of the file.
  UInt32 fMode;  /// @brief File mode. (read, write, execute, etc).

  /// @brief Extents system by using blocks
  /// @details Using an offset to ask fBase, and fLength to compute each slice's length.
  UInt32 fOffsetSliceLow;
  UInt32 fOffsetSliceHigh;

  Char fPad[437];
};

enum {
  kOpenHeFSInvalidColor = 0,
  kOpenHeFSRed          = 100,
  kOpenHeFSBlack,
  kOpenHeFSColorCount,
};

/// @brief OpenHeFS directory node.
/// @details This structure is used to store the directory information of a file.
/// @note The directory node is a special type of INode that contains the directory entries.
struct PACKED HEFS_INDEX_NODE_DIRECTORY final {
  UInt64 fHashPath;  /// @brief Directory path as FNV hash.

  UInt32 fFlags;       /// @brief File flags.
  UInt16 fReserved;    /// @note Reserved for future use.
  UInt32 fEntryCount;  /// @brief Entry Count of this directory inode.
  UInt32 fChecksum;    /// @brief Checksum of the file, index node checksum.

  ATime fCreated, fAccessed, fModified,
      fDeleted;       /// @brief File timestamps and allocation status.
  UInt32 fUID, fGID;  /// @brief User ID and Group ID of the file.
  UInt32 fMode;       /// @brief File mode. (read, write, execute, etc).

  /// @note These slices are organized as:
  /// [0] = OFFSET
  /// [1] = SIZE
  /// @note Thus the += 2 when iterating over them.
  UInt64 fINSlices[kOpenHeFSSliceCount];  /// @brief Start of the index node.

  UInt8 fColor;                         /// @brief Color of the node. (Red or Black).
  Lba   fNext, fPrev, fChild, fParent;  /// @brief Red-black tree pointers.

  Char fPad[285];
};
}  // namespace Kernel

namespace Kernel::Detail {
/// @brief OpenHeFS get year from ATime.
/// @param raw_atime the raw ATime value.
/// @return the year value.
/// @note The year is stored in the upper 32 bits of the ATime value.
inline UInt32 hefs_year_get(ATime raw_atime) {
  return (raw_atime) >> 32;
}

/// @brief OpenHeFS get month from ATime.
/// @param raw_atime the raw ATime value.
/// @return the month value.
/// @note The month is stored in the upper 24 bits of the ATime value.
inline UInt32 hefs_month_get(ATime raw_atime) {
  return (raw_atime) >> 24;
}

/// @brief OpenHeFS get day from ATime.
/// @param raw_atime the raw ATime value.
/// @return the day value.
/// @note The day is stored in the upper 16 bits of the ATime value.
inline UInt32 hefs_day_get(ATime raw_atime) {
  return (raw_atime) >> 16;
}

/// @brief OpenHeFS get hour from ATime.
/// @param raw_atime the raw ATime value.
/// @return the hour value.
/// @note The hour is stored in the upper 8 bits of the ATime value.
inline UInt32 hefs_hour_get(ATime raw_atime) {
  return (raw_atime) >> 8;
}

/// @brief OpenHeFS get minute from ATime.
/// @param raw_atime the raw ATime value.
/// @return the minute value.
/// @note The minute is stored in the lower 8 bits of the ATime value.
inline UInt32 hefs_minute_get(ATime raw_atime) {
  return (raw_atime) & 0xFF;
}

inline constexpr UInt32 kOpenHeFSBaseYear   = 1970;
inline constexpr UInt32 kOpenHeFSBaseMonth  = 1;
inline constexpr UInt32 kOpenHeFSBaseDay    = 1;
inline constexpr UInt32 kOpenHeFSBaseHour   = 0;
inline constexpr UInt32 kOpenHeFSBaseMinute = 0;

inline const Char* hefs_status_to_string(UInt16 status) {
  switch (status) {
    case kOpenHeFSStatusUnlocked:
      return "Unlocked";
    case kOpenHeFSStatusLocked:
      return "Locked";
    case kOpenHeFSStatusError:
      return "Error";
    case kOpenHeFSStatusInvalid:
      return "Invalid";
    default:
      return "Unknown";
  }
}

inline const Char* hefs_drive_kind_to_string(UInt8 kind) {
  switch (kind) {
    case kOpenHeFSHardDrive:
      return "Hard Drive";
    case kOpenHeFSSolidStateDrive:
      return "Solid State Drive";
    case kOpenHeFSOpticalDrive:
      return "Optical Drive";
    case kOpenHeFSMassStorageDevice:
      return "Mass Storage Device";
    case kOpenHeFSScsiDrive:
      return "SCSI/SAS Drive";
    case kOpenHeFSFlashDrive:
      return "Flash Drive";
    case kOpenHeFSUnknown:
    default:
      return "Unknown";
  }
}

inline const Char* hefs_encoding_to_string(UInt8 encoding) {
  switch (encoding) {
    case kOpenHeFSEncodingFlagsUTF8:
      return "UTF-8";
    case kOpenHeFSEncodingFlagsUTF16:
      return "UTF-16";
    case kOpenHeFSEncodingFlagsUTF32:
      return "UTF-32";
    case kOpenHeFSEncodingFlagsUTF16BE:
      return "UTF-16BE";
    case kOpenHeFSEncodingFlagsUTF16LE:
      return "UTF-16LE";
    case kOpenHeFSEncodingFlagsUTF32BE:
      return "UTF-32BE";
    case kOpenHeFSEncodingFlagsUTF32LE:
      return "UTF-32LE";
    case kOpenHeFSEncodingFlagsUTF8BE:
      return "UTF-8BE";
    case kOpenHeFSEncodingFlagsUTF8LE:
      return "UTF-8LE";
    default:
      return "Unknown";
  }
}

inline const Char* hefs_file_kind_to_string(UInt16 kind) {
  switch (kind) {
    case kOpenHeFSFileKindRegular:
      return "Regular File";
    case kOpenHeFSFileKindDirectory:
      return "Directory";
    case kOpenHeFSFileKindBlock:
      return "Block Device";
    case kOpenHeFSFileKindCharacter:
      return "Character Device";
    case kOpenHeFSFileKindFIFO:
      return "FIFO";
    case kOpenHeFSFileKindSocket:
      return "Socket";
    case kOpenHeFSFileKindSymbolicLink:
      return "Symbolic Link";
    case kOpenHeFSFileKindUnknown:
    default:
      return "Unknown";
  }
}

inline const Char* hefs_file_flags_to_string(UInt32 flags) {
  switch (flags) {
    case kOpenHeFSFlagsNone:
      return "No Flags";
    case kOpenHeFSFlagsReadOnly:
      return "Read Only";
    case kOpenHeFSFlagsHidden:
      return "Hidden";
    case kOpenHeFSFlagsSystem:
      return "System";
    case kOpenHeFSFlagsArchive:
      return "Archive";
    case kOpenHeFSFlagsDevice:
      return "Device";
    default:
      return "Unknown";
  }
}
}  // namespace Kernel::Detail

namespace Kernel {
/// @brief OpenHeFS filesystem parser class.
/// @details This class is used to parse the OpenHeFS filesystem.
class HeFileSystemParser final {
 public:
  HeFileSystemParser()  = default;
  ~HeFileSystemParser() = default;

 public:
  HeFileSystemParser(const HeFileSystemParser&)            = delete;
  HeFileSystemParser& operator=(const HeFileSystemParser&) = delete;

  HeFileSystemParser(HeFileSystemParser&&)            = delete;
  HeFileSystemParser& operator=(HeFileSystemParser&&) = delete;

 public:
  /// @brief Make a EPM+OpenHeFS drive out of the disk.
  /// @param drive The drive to write on.
  /// @return If it was sucessful, see err_local_get().
  _Output Bool Format(_Input _Output DriveTrait* drive, _Input const Int32 flags,
                      const Utf8Char* part_name);

  _Output Bool CreateINodeDirectory(_Input DriveTrait* drive, _Input const Int32 flags,
                                    const Utf8Char* dir);

  _Output Bool RemoveINodeDirectory(_Input DriveTrait* drive, _Input const Int32 flags,
                                    const Utf8Char* dir);

  _Output Bool CreateINode(_Input DriveTrait* drive, _Input const Int32 flags, const Utf8Char* dir,
                           const Utf8Char* name, const UInt8 kind);

  _Output Bool DeleteINode(_Input DriveTrait* drive, _Input const Int32 flags, const Utf8Char* dir,
                           const Utf8Char* name, const UInt8 kind);

  _Output Bool INodeManip(_Input DriveTrait* drive, VoidPtr block, SizeT block_sz,
                          const Utf8Char* dir, const Utf8Char* name, const UInt8 kind,
                          const BOOL input);

 private:
  _Output Bool INodeCtlManip(_Input DriveTrait* drive, _Input const Int32 flags,
                             const Utf8Char* dir, const Utf8Char* name, const BOOL delete_or_create,
                             const UInt8 kind);

  _Output Bool INodeDirectoryCtlManip(_Input DriveTrait* drive, _Input const Int32 flags,
                                      const Utf8Char* dir, const BOOL delete_or_create);
};

namespace OpenHeFS {

  /// @brief Initialize OpenHeFS inside the main disk.
  /// @return Whether it successfuly formated it or not.
  Boolean fs_init_openhefs(Void);
}  // namespace OpenHeFS
}  // namespace Kernel

#endif
