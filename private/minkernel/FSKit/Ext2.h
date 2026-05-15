// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef FSKIT_EXT2_H
#define FSKIT_EXT2_H

#include <CompilerKit/CompilerKit.h>
#include <KernelKit/DriveMgr.h>
#include <NeKit/Config.h>
#include <NeKit/KString.h>
#include <hint/CompilerHint.h>

namespace Kernel {

inline constexpr UInt16 kExt2FSMagic             = 0xEF53;
inline constexpr UInt32 kExt2FSSuperblockOffset  = 1024;
inline constexpr UInt32 kExt2FSSuperblockSize    = 1024;
inline constexpr UInt32 kExt2FSRootInodeNumber   = 2;
inline constexpr UInt32 kExt2FSGoodOldInodeSize  = 128;
inline constexpr UInt32 kExt2FSBlockSizeBase     = 1024;
inline constexpr UInt32 kExt2FSMaxFileNameLen    = 255;
inline constexpr UInt32 kExt2FSDirEntryHeaderLen = 8;
inline constexpr UInt32 kExt2FSDirectBlocks      = 12;
inline constexpr UInt32 kExt2FSSingleIndirect    = 12;
inline constexpr UInt32 kExt2FSDoubleIndirect    = 13;
inline constexpr UInt32 kExt2FSTripleIndirect    = 14;
inline constexpr UInt32 kExt2FSBlockPointerCount = 15;

inline constexpr UInt32 kExt2FSRev0 = 0;
inline constexpr UInt32 kExt2FSRev1 = 1;

inline constexpr UInt32 kExt2FeatureCompatDirPrealloc   = 0x0001;
inline constexpr UInt32 kExt2FeatureCompatImagicInodes  = 0x0002;
inline constexpr UInt32 kExt2FeatureCompatHasJournal    = 0x0004;
inline constexpr UInt32 kExt2FeatureCompatExtAttr       = 0x0008;
inline constexpr UInt32 kExt2FeatureCompatResizeInode   = 0x0010;
inline constexpr UInt32 kExt2FeatureCompatDirIndex      = 0x0020;

inline constexpr UInt32 kExt2FeatureIncompatCompression = 0x0001;
inline constexpr UInt32 kExt2FeatureIncompatFileType    = 0x0002;
inline constexpr UInt32 kExt2FeatureIncompatRecover     = 0x0004;
inline constexpr UInt32 kExt2FeatureIncompatJournalDev  = 0x0008;
inline constexpr UInt32 kExt2FeatureIncompatMetaBG      = 0x0010;

inline constexpr UInt32 kExt2FSIncompatSupported =
    kExt2FeatureIncompatFileType | kExt2FeatureIncompatMetaBG;

inline constexpr UInt32 kExt2FeatureRoCompatSparseSuper = 0x0001;
inline constexpr UInt32 kExt2FeatureRoCompatLargeFile   = 0x0002;
inline constexpr UInt32 kExt2FeatureRoCompatBTreeDir    = 0x0004;

inline constexpr UInt32 kExt2FSRoCompatSupported =
    kExt2FeatureRoCompatSparseSuper | kExt2FeatureRoCompatLargeFile;

enum : UInt8 {
  kExt2FileTypeUnknown      = 0,
  kExt2FileTypeRegular      = 1,
  kExt2FileTypeDirectory    = 2,
  kExt2FileTypeCharDevice   = 3,
  kExt2FileTypeBlockDevice  = 4,
  kExt2FileTypeFIFO         = 5,
  kExt2FileTypeSocket       = 6,
  kExt2FileTypeSymbolicLink = 7,
  kExt2FileTypeCount,
};

inline constexpr UInt16 kExt2ModeFmtMask    = 0xF000;
inline constexpr UInt16 kExt2ModeFmtSocket  = 0xC000;
inline constexpr UInt16 kExt2ModeFmtSymlink = 0xA000;
inline constexpr UInt16 kExt2ModeFmtReg     = 0x8000;
inline constexpr UInt16 kExt2ModeFmtBlk     = 0x6000;
inline constexpr UInt16 kExt2ModeFmtDir     = 0x4000;
inline constexpr UInt16 kExt2ModeFmtChr     = 0x2000;
inline constexpr UInt16 kExt2ModeFmtFifo    = 0x1000;

struct PACKED EXT2_GROUP_DESCRIPTOR final {
  UInt32 fBlockBitmap;
  UInt32 fInodeBitmap;
  UInt32 fInodeTable;
  UInt16 fFreeBlocksCount;
  UInt16 fFreeInodesCount;
  UInt16 fUsedDirsCount;
  UInt16 fPad;
  UInt32 fReserved[3];
};

static_assert(sizeof(EXT2_GROUP_DESCRIPTOR) == 32);

struct PACKED EXT2_SUPER_BLOCK final {
  UInt32 fInodeCount;
  UInt32 fBlockCount;
  UInt32 fReservedBlockCount;
  UInt32 fFreeBlockCount;
  UInt32 fFreeInodeCount;
  UInt32 fFirstDataBlock;
  UInt32 fLogBlockSize;
  UInt32 fLogFragmentSize;
  UInt32 fBlocksPerGroup;
  UInt32 fFragmentsPerGroup;
  UInt32 fInodesPerGroup;
  UInt32 fMountTime;
  UInt32 fWriteTime;
  UInt16 fMountCount;
  UInt16 fMaxMountCount;
  UInt16 fMagic;
  UInt16 fState;
  UInt16 fErrors;
  UInt16 fMinorRevision;
  UInt32 fLastCheck;
  UInt32 fCheckInterval;
  UInt32 fCreatorOS;
  UInt32 fRevisionLevel;
  UInt16 fDefaultUID;
  UInt16 fDefaultGID;
  UInt32 fFirstInode;
  UInt16 fInodeSize;
  UInt16 fBlockGroupNumber;
  UInt32 fFeatureCompat;
  UInt32 fFeatureIncompat;
  UInt32 fFeatureROCompat;
  UInt8  fUUID[16];
  Char   fVolumeName[16];
  Char   fLastMounted[64];
  UInt32 fAlgoBitmap;
  UInt8  fPreallocBlocks;
  UInt8  fPreallocDirBlocks;
  UInt16 fReservedGDTBlocks;
  UInt8  fJournalUUID[16];
  UInt32 fJournalInode;
  UInt32 fJournalDevice;
  UInt32 fLastOrphan;
  UInt32 fHashSeed[4];
  UInt8  fDefHashVersion;
  UInt8  fReservedCharPad;
  UInt16 fReservedWordPad;
  UInt32 fDefaultMountOpts;
  UInt32 fFirstMetaBlockGroup;
  UInt8  fReserved[760];
};

static_assert(sizeof(EXT2_SUPER_BLOCK) == kExt2FSSuperblockSize);

struct PACKED EXT2_INODE final {
  UInt16 fMode;
  UInt16 fUID;
  // High 32 bits live in fDirACL on LARGE_FILE volumes for regular files.
  UInt32 fSizeLow;
  UInt32 fAccessTime;
  UInt32 fCreateTime;
  UInt32 fModifyTime;
  UInt32 fDeleteTime;
  UInt16 fGID;
  UInt16 fLinksCount;
  // Count is in 512-byte sectors, not filesystem blocks.
  UInt32 fBlocks;
  UInt32 fFlags;
  UInt32 fOSD1;
  UInt32 fBlock[kExt2FSBlockPointerCount];
  UInt32 fGeneration;
  UInt32 fFileACL;
  UInt32 fDirACL;
  UInt32 fFragmentAddr;
  UInt8  fOSD2[12];
};

static_assert(sizeof(EXT2_INODE) == kExt2FSGoodOldInodeSize);

// On-disk record is variable-length: header + name (fNameLength bytes),
// padded up so fRecordLength is 4-byte aligned. The last record in a
// block has fRecordLength inflated to cover the block tail.
struct PACKED EXT2_DIR_ENTRY_HEADER final {
  UInt32 fInode;
  UInt16 fRecordLength;
  UInt8  fNameLength;
  UInt8  fFileType;
};

static_assert(sizeof(EXT2_DIR_ENTRY_HEADER) == kExt2FSDirEntryHeaderLen);

struct Ext2Node final {
  UInt32     fInodeNumber{0};
  EXT2_INODE fInode{};
  UInt64     fCursor{0};
};

inline UInt64 ext2_inode_size(const EXT2_INODE& n, UInt32 roCompat) {
  const UInt64 lo = n.fSizeLow;
  if ((n.fMode & kExt2ModeFmtMask) == kExt2ModeFmtReg &&
      (roCompat & kExt2FeatureRoCompatLargeFile) != 0)
    return lo | (static_cast<UInt64>(n.fDirACL) << 32);
  return lo;
}

inline UInt8 ext2_inode_type(const EXT2_INODE& n) {
  switch (n.fMode & kExt2ModeFmtMask) {
    case kExt2ModeFmtReg:     return kExt2FileTypeRegular;
    case kExt2ModeFmtDir:     return kExt2FileTypeDirectory;
    case kExt2ModeFmtChr:     return kExt2FileTypeCharDevice;
    case kExt2ModeFmtBlk:     return kExt2FileTypeBlockDevice;
    case kExt2ModeFmtFifo:    return kExt2FileTypeFIFO;
    case kExt2ModeFmtSocket:  return kExt2FileTypeSocket;
    case kExt2ModeFmtSymlink: return kExt2FileTypeSymbolicLink;
    default:                  return kExt2FileTypeUnknown;
  }
}

}  // namespace Kernel

#endif
