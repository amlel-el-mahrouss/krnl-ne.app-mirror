// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel


#ifndef __NE_MINIMAL_OS__
#ifdef __FSKIT_INCLUDES_EXT2__

#include <FSKit/Ext2+IFS.h>
#include <FSKit/Ext2.h>
#include <KernelKit/DebugOutput.h>
#include <KernelKit/FileMgr.h>
#include <KernelKit/HeapMgr.h>
#include <NeKit/ErrorOr.h>
#include <NeKit/KString.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/Utils.h>

/// @note There's no current maintainer of Ext2, the position is vacant.
/// @note AMLALE: @0xf00sec has written this driver, and I @amlel-el-mahrouss is currently working on adding triple indirect indices.
/// @note AMLALE: This driver is too verbose, we need a complete overhaul of it.

constexpr static UInt32 EXT2_DIRECT_BLOCKS                           = 12;
constexpr static UInt32 EXT2_SINGLE_INDIRECT_INDEX                   = 12;
constexpr static UInt32 EXT2_DOUBLE_INDIRECT_INDEX                   = 13;
constexpr static UInt32 EXT2_TRIPLE_INDIRECT_INDEX = 14;
constexpr static UInt32 EXT2_ROOT_INODE                              = 2;
constexpr ATTRIBUTE(unused) static UInt32 EXT2_SUPERBLOCK_BLOCK      = 1;
constexpr static UInt32 EXT2_GROUP_DESC_BLOCK_SMALL                  = 2;
constexpr static UInt32 EXT2_GROUP_DESC_BLOCK_LARGE                  = 1;

namespace Detail {
static inline SizeT ext2_min(SizeT a, SizeT b) {
  return a < b ? a : b;
}

struct Ext2GroupInfo final {
  EXT2_GROUP_DESCRIPTOR* groupDesc;
  UInt32                 groupDescriptorBlock;
  UInt32                 offsetInGroupDescBlock;
  UInt8*                 blockBuffer;
};
}  // namespace Detail

// Convert EXT2 block number -> LBA (sector index) for Drive I/O.
static inline UInt32 ext2_block_to_lba(Ext2Context* ctx, UInt32 blockNumber) {
  if (!ctx || !ctx->drive) return 0;
  UInt32 blockSize       = ctx->BlockSize();
  UInt32 sectorSize      = ctx->drive->fSectorSz;
  UInt32 sectorsPerBlock = blockSize / sectorSize;
  return blockNumber * sectorsPerBlock;
}

// Read a block and return a pointer to its content
static ErrorOr<UInt32*> ext2_read_block_ptr(Ext2Context* ctx, UInt32 blockNumber) {
  if (!ctx || !ctx->drive || !ctx->superblock) return ErrorOr<UInt32*>(kErrorInvalidData);

  UInt32 blockSize = ctx->BlockSize();
  auto   buf       = (UInt32*) mm_alloc_ptr(blockSize, true, false);
  if (!buf) return ErrorOr<UInt32*>(kErrorHeapOutOfMemory);

  UInt32 lba = ext2_block_to_lba(ctx, blockNumber);
  if (!ext2_read_block(ctx->drive, lba, buf, blockSize)) {
    mm_free_ptr(buf);
    return ErrorOr<UInt32*>(kErrorDisk);
  }
  return ErrorOr<UInt32*>(buf);
}

// Get the block address for a given logical block index
static ErrorOr<UInt32> ext2_get_block_address(Ext2Context* ctx, Ext2Node* node,
                                              UInt32 logicalIndex) {
  if (!ctx || !node || !ctx->drive) return ErrorOr<UInt32>(kErrorInvalidData);

  UInt32 blockSize        = ctx->BlockSize();
  UInt32 pointersPerBlock = blockSize / sizeof(UInt32);

  // Direct blocks
  if (logicalIndex < EXT2_DIRECT_BLOCKS) {
    UInt32 bn = node->inode.fBlock[logicalIndex];
    if (bn == 0) return ErrorOr<UInt32>(kErrorInvalidData);
    return ErrorOr<UInt32>(bn);
  }

  // Single indirect blocks
  if (logicalIndex < (EXT2_DIRECT_BLOCKS + pointersPerBlock)) {
    UInt32 iblock = node->inode.fBlock[EXT2_SINGLE_INDIRECT_INDEX];
    if (iblock == 0) return ErrorOr<UInt32>(kErrorInvalidData);

    auto res = ext2_read_block_ptr(ctx, iblock);
    if (!res) return ErrorOr<UInt32>(res.Error());

    // Using dereference operator
    UInt32* ptr = *res.Leak();  // operator* returns T (UInt32*)

    UInt32 val = ptr[logicalIndex - EXT2_DIRECT_BLOCKS];
    mm_free_ptr(ptr);

    if (val == 0) return ErrorOr<UInt32>(kErrorInvalidData);
    return ErrorOr<UInt32>(val);
  }

  // Double indirect blocks
  UInt32 doubleStart = EXT2_DIRECT_BLOCKS + pointersPerBlock;
  UInt32 doubleSpan  = pointersPerBlock * pointersPerBlock;
  if (logicalIndex < (doubleStart + doubleSpan)) {
    UInt32 db = node->inode.fBlock[EXT2_DOUBLE_INDIRECT_INDEX];
    if (db == 0) return ErrorOr<UInt32>(kErrorInvalidData);

    auto dblRes = ext2_read_block_ptr(ctx, db);
    if (!dblRes) return ErrorOr<UInt32>(dblRes.Error());

    UInt32* dblPtr = *dblRes.Leak();

    UInt32 idxWithin      = logicalIndex - doubleStart;
    UInt32 firstIdx       = idxWithin / pointersPerBlock;
    UInt32 secondIdx      = idxWithin % pointersPerBlock;
    UInt32 singleBlockNum = dblPtr[firstIdx];

    mm_free_ptr(dblPtr);
    if (singleBlockNum == 0) return ErrorOr<UInt32>(kErrorInvalidData);

    auto singleRes = ext2_read_block_ptr(ctx, singleBlockNum);
    if (!singleRes) return ErrorOr<UInt32>(singleRes.Error());

    UInt32* singlePtr = *singleRes.Leak();
    UInt32  val       = singlePtr[secondIdx];
    mm_free_ptr(singlePtr);

    if (val == 0) return ErrorOr<UInt32>(kErrorInvalidData);
    return ErrorOr<UInt32>(val);
  }

  return ErrorOr<UInt32>(kErrorUnimplemented);
}

static ErrorOr<voidPtr> ext2_read_inode_data(Ext2Context* ctx, Ext2Node* node, SizeT size) {
  if (!ctx || !ctx->drive || !node || size == 0) return ErrorOr<voidPtr>(1);

  auto  blockSize   = ctx->BlockSize();
  SizeT available   = (node->inode.fSize > node->cursor) ? (node->inode.fSize - node->cursor) : 0;
  SizeT bytesToRead = (size < available) ? size : available;
  if (bytesToRead == 0) return ErrorOr<voidPtr>(2);  // nothing to read

  auto buffer = mm_alloc_ptr(bytesToRead, true, false);
  if (!buffer) return ErrorOr<voidPtr>(3);  // allocation failed

  UInt32 currentOffset = node->cursor;
  SizeT  remaining     = bytesToRead;
  UInt8* dest          = reinterpret_cast<UInt8*>(buffer);

  while (remaining > 0) {
    UInt32 logicalIndex  = currentOffset / blockSize;
    UInt32 offsetInBlock = currentOffset % blockSize;

    auto phys = ext2_get_block_address(ctx, node, logicalIndex);
    if (phys.HasError()) {
      mm_free_ptr(buffer);
      return ErrorOr<voidPtr>(phys.Error());
    }

    auto   blockNumber = phys.Value();
    UInt32 lba         = ext2_block_to_lba(ctx, blockNumber);

    auto blockBuf = mm_alloc_ptr(blockSize, true, false);
    if (!blockBuf) {
      mm_free_ptr(buffer);
      return ErrorOr<voidPtr>(4);  // block buffer allocation failed
    }

    if (!ext2_read_block(ctx->drive, lba, blockBuf, blockSize)) {
      mm_free_ptr(blockBuf);
      mm_free_ptr(buffer);
      return ErrorOr<voidPtr>(5);  // block read failed
    }

    SizeT chunk = ::Detail::ext2_min(remaining, blockSize - offsetInBlock);
    rt_copy_memory_safe(static_cast<void*>(static_cast<UInt8*>(blockBuf) + offsetInBlock),
                        static_cast<void*>(dest), chunk, chunk);

    mm_free_ptr(blockBuf);

    currentOffset += static_cast<UInt32>(chunk);
    dest += chunk;
    remaining -= chunk;
  }

  node->cursor += static_cast<UInt32>(bytesToRead);
  return ErrorOr<voidPtr>(buffer);
}

// Get group descriptor information for a given block/inode number
static ErrorOr<::Detail::Ext2GroupInfo*> ext2_get_group_descriptor_info(Ext2Context* ctx,
                                                                        UInt32 targetBlockOrInode) {
  if (!ctx || !ctx->superblock || !ctx->drive)
    return ErrorOr<::Detail::Ext2GroupInfo*>(kErrorInvalidData);

  UInt32 blockSize      = ctx->BlockSize();
  UInt32 blocksPerGroup = ctx->superblock->fBlocksPerGroup;
  UInt32 inodesPerGroup = ctx->superblock->fInodesPerGroup;
  UInt32 totalBlocks    = ctx->superblock->fBlockCount;
  UInt32 totalInodes    = ctx->superblock->fInodeCount;

  if (blocksPerGroup == 0 || inodesPerGroup == 0)
    return ErrorOr<::Detail::Ext2GroupInfo*>(kErrorInvalidData);

  // block group index
  UInt32 groupIndex = 0;
  if (targetBlockOrInode == 0) {
    groupIndex = 0;
  } else if (targetBlockOrInode <= totalInodes) {
    // 1-based
    groupIndex = (targetBlockOrInode - 1) / inodesPerGroup;
  } else {
    // EXT2 block number
    if (targetBlockOrInode < ctx->superblock->fFirstDataBlock) {
      groupIndex = 0;
    } else {
      groupIndex = (targetBlockOrInode - ctx->superblock->fFirstDataBlock) / blocksPerGroup;
    }
  }

  // Calculate number of block groups
  UInt32 groupsCount = static_cast<UInt32>((totalBlocks + blocksPerGroup - 1) / blocksPerGroup);
  if (groupIndex >= groupsCount) return ErrorOr<::Detail::Ext2GroupInfo*>(kErrorInvalidData);

  // Determine GDT start block
  UInt32 gdtStartBlock =
      (blockSize == 1024) ? EXT2_GROUP_DESC_BLOCK_SMALL : EXT2_GROUP_DESC_BLOCK_LARGE;

  // Compute byte offset of descriptor within the GDT
  const UInt32 descSize       = sizeof(EXT2_GROUP_DESCRIPTOR);
  UInt64       descByteOffset = static_cast<UInt64>(groupIndex) * descSize;

  // Which EXT2 block contains that descriptor?
  UInt32 blockOffsetWithinGdt   = static_cast<UInt32>(descByteOffset / blockSize);
  UInt32 offsetInGroupDescBlock = static_cast<UInt32>(descByteOffset % blockSize);
  UInt32 groupDescriptorBlock   = gdtStartBlock + blockOffsetWithinGdt;

  // Allocate buffer and read the block containing the descriptor
  auto blockBuffer = mm_alloc_ptr(blockSize, true, false);
  if (!blockBuffer) return ErrorOr<::Detail::Ext2GroupInfo*>(kErrorHeapOutOfMemory);

  UInt32 groupDescriptorLba = ext2_block_to_lba(ctx, groupDescriptorBlock);
  if (!ext2_read_block(ctx->drive, groupDescriptorLba, blockBuffer, blockSize)) {
    mm_free_ptr(blockBuffer);
    return ErrorOr<::Detail::Ext2GroupInfo*>(kErrorDisk);
  }

  auto groupInfo =
      (::Detail::Ext2GroupInfo*) mm_alloc_ptr(sizeof(::Detail::Ext2GroupInfo), true, false);
  if (!groupInfo) {
    mm_free_ptr(blockBuffer);
    return ErrorOr<::Detail::Ext2GroupInfo*>(kErrorHeapOutOfMemory);
  }

  groupInfo->groupDesc = reinterpret_cast<EXT2_GROUP_DESCRIPTOR*>(
      reinterpret_cast<UInt8*>(blockBuffer) + offsetInGroupDescBlock);
  groupInfo->groupDescriptorBlock   = groupDescriptorBlock;
  groupInfo->offsetInGroupDescBlock = offsetInGroupDescBlock;
  groupInfo->blockBuffer            = reinterpret_cast<UInt8*>(blockBuffer);

  return ErrorOr<::Detail::Ext2GroupInfo*>(groupInfo);
}

// Allocate a new block
inline ErrorOr<UInt32> ext2_alloc_block(Ext2Context* ctx, EXT2_GROUP_DESCRIPTOR* groupDesc) {
  if (!ctx || !ctx->superblock || !groupDesc) return ErrorOr<UInt32>(kErrorInvalidData);

  UInt32 blockSize = ctx->BlockSize();

  // for the bitmap
  auto bitmap = mm_alloc_ptr(blockSize, true, false);
  if (!bitmap) return ErrorOr<UInt32>(kErrorHeapOutOfMemory);

  // Read block bitmap
  if (!ext2_read_block(ctx->drive, groupDesc->fBlockBitmap, bitmap, blockSize)) {
    mm_free_ptr(bitmap);
    return ErrorOr<UInt32>(kErrorDisk);
  }

  // bit = 0
  for (UInt32 byteIdx = 0; byteIdx < blockSize; ++byteIdx) {
    auto byte = reinterpret_cast<unsigned char*>(bitmap)[byteIdx];
    if (byte != 0xFF) {
      for (int bit = 0; bit < 8; ++bit) {
        if (!(byte & (1 << bit))) {
          // Mark bit as used
          reinterpret_cast<unsigned char*>(bitmap)[byteIdx] |= (1 << bit);

          // Compute block number
          UInt32 blockNumber = byteIdx * 8 + bit;

          // Write bitmap back
          if (!ext2_write_block(ctx->drive, groupDesc->fBlockBitmap, bitmap, blockSize)) {
            mm_free_ptr(bitmap);
            return ErrorOr<UInt32>(kErrorDisk);
          }

          // Update group descriptor free count
          groupDesc->fFreeBlocksCount--;
          mm_free_ptr(bitmap);
          return ErrorOr<UInt32>(blockNumber);
        }
      }
    }
  }

  mm_free_ptr(bitmap);
  return ErrorOr<UInt32>(kErrorDiskIsFull);
}

// Indirect blocks
static ErrorOr<Void*> ext2_set_block_address(Ext2Context* ctx, Ext2Node* node,
                                             UInt32 logicalBlockIndex, UInt32 physicalBlockNumber) {
  using namespace Kernel;

  if (!ctx || !ctx->drive || !node) return ErrorOr<Void*>(kErrorInvalidData);

  auto   blockSize             = ctx->BlockSize();
  UInt32 blocksPerPointerBlock = blockSize / sizeof(UInt32);

  // Direct blocks
  if (logicalBlockIndex < EXT2_DIRECT_BLOCKS) {
    node->inode.fBlock[logicalBlockIndex] = physicalBlockNumber;
    return ErrorOr<Void*>(nullptr);
  }

  // Single indirect blocks
  if (logicalBlockIndex < EXT2_DIRECT_BLOCKS + blocksPerPointerBlock) {
    if (node->inode.fBlock[EXT2_SINGLE_INDIRECT_INDEX] == 0) {
      auto groupInfoRes = ext2_get_group_descriptor_info(ctx, node->inodeNumber);
      if (groupInfoRes.HasError()) return ErrorOr<Void*>(groupInfoRes.Error());

      auto groupInfo   = groupInfoRes.Leak().Leak();  // Ref<::Detail::Ext2GroupInfo*>
      auto newBlockRes = ext2_alloc_block(ctx, groupInfo->groupDesc);
      if (newBlockRes.HasError()) {
        mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
        mm_free_ptr(groupInfo);
        return ErrorOr<Void*>(newBlockRes.Error());
      }

      node->inode.fBlock[EXT2_SINGLE_INDIRECT_INDEX] = newBlockRes.Leak().Leak();

      UInt32 gdtLba = ext2_block_to_lba(ctx, groupInfo->groupDescriptorBlock);
      if (!ext2_write_block(ctx->drive, gdtLba, groupInfo->blockBuffer, blockSize)) {
        mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
        mm_free_ptr(groupInfo);
        return ErrorOr<Void*>(kErrorDisk);
      }

      mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
      mm_free_ptr(groupInfo);

      // Zero out new indirect block
      auto zeroBuf = mm_alloc_ptr(blockSize, true, false);
      if (!zeroBuf) return ErrorOr<Void*>(kErrorHeapOutOfMemory);

      rt_zero_memory(zeroBuf, blockSize);
      UInt32 indirectLba = ext2_block_to_lba(ctx, node->inode.fBlock[EXT2_SINGLE_INDIRECT_INDEX]);
      if (!ext2_write_block(ctx->drive, indirectLba, zeroBuf, blockSize)) {
        mm_free_ptr(zeroBuf);
        return ErrorOr<Void*>(kErrorDisk);
      }

      mm_free_ptr(zeroBuf);
    }

    // Read, modify, and write single indirect block
    auto indirectRes = ext2_read_block_ptr(ctx, node->inode.fBlock[EXT2_SINGLE_INDIRECT_INDEX]);
    if (indirectRes.HasError()) return ErrorOr<Void*>(indirectRes.Error());

    UInt32* indirectPtr = indirectRes.Leak().Leak();  // Ref<UInt32*>
    indirectPtr[logicalBlockIndex - EXT2_DIRECT_BLOCKS] = physicalBlockNumber;

    UInt32 indirectLba = ext2_block_to_lba(ctx, node->inode.fBlock[EXT2_SINGLE_INDIRECT_INDEX]);
    if (!ext2_write_block(ctx->drive, indirectLba, indirectPtr, blockSize)) {
      mm_free_ptr(indirectPtr);
      return ErrorOr<Void*>(kErrorDisk);
    }

    mm_free_ptr(indirectPtr);
    return ErrorOr<Void*>(nullptr);
  }

  // Double
  UInt32 doubleStart = EXT2_DIRECT_BLOCKS + blocksPerPointerBlock;
  UInt32 doubleSpan  = blocksPerPointerBlock * blocksPerPointerBlock;
  UInt32 tripleSpan  = blocksPerPointerBlock * blocksPerPointerBlock * blocksPerPointerBlock;

  if (logicalBlockIndex < doubleStart + doubleSpan) {
    if (node->inode.fBlock[EXT2_DOUBLE_INDIRECT_INDEX] == 0) {
      auto groupInfoRes = ext2_get_group_descriptor_info(ctx, node->inodeNumber);
      if (groupInfoRes.HasError()) return ErrorOr<Void*>(groupInfoRes.Error());

      auto groupInfo   = groupInfoRes.Leak().Leak();
      auto newBlockRes = ext2_alloc_block(ctx, groupInfo->groupDesc);
      if (newBlockRes.HasError()) {
        mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
        mm_free_ptr(groupInfo);
        return ErrorOr<Void*>(newBlockRes.Error());
      }

      node->inode.fBlock[EXT2_DOUBLE_INDIRECT_INDEX] = newBlockRes.Leak().Leak();

      UInt32 gdtLba = ext2_block_to_lba(ctx, groupInfo->groupDescriptorBlock);
      if (!ext2_write_block(ctx->drive, gdtLba, groupInfo->blockBuffer, blockSize)) {
        mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
        mm_free_ptr(groupInfo);
        return ErrorOr<Void*>(kErrorDisk);
      }

      mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
      mm_free_ptr(groupInfo);

      // Zero new double-indirect block
      auto zeroBuf = mm_alloc_ptr(blockSize, true, false);
      if (!zeroBuf) return ErrorOr<Void*>(kErrorHeapOutOfMemory);

      rt_zero_memory(zeroBuf, blockSize);
      UInt32 dblLba = ext2_block_to_lba(ctx, node->inode.fBlock[EXT2_DOUBLE_INDIRECT_INDEX]);
      if (!ext2_write_block(ctx->drive, dblLba, zeroBuf, blockSize)) {
        mm_free_ptr(zeroBuf);
        return ErrorOr<Void*>(kErrorDisk);
      }

      mm_free_ptr(zeroBuf);
    }

    // Compute indices
    UInt32 idxWithin = logicalBlockIndex - doubleStart;
    UInt32 firstIdx  = idxWithin / blocksPerPointerBlock;
    UInt32 secondIdx = idxWithin % blocksPerPointerBlock;

    auto doubleRes = ext2_read_block_ptr(ctx, node->inode.fBlock[EXT2_DOUBLE_INDIRECT_INDEX]);

    if (doubleRes.HasError()) return ErrorOr<Void*>(doubleRes.Error());

    UInt32* doublePtr           = doubleRes.Leak().Leak();
    UInt32  singleIndirectBlock = doublePtr[firstIdx];

    // Allocate single-indirect if missing
    if (singleIndirectBlock == 0) {
      auto groupInfoRes = ext2_get_group_descriptor_info(ctx, node->inodeNumber);

      if (groupInfoRes.HasError()) {
        mm_free_ptr(doublePtr);
        return ErrorOr<Void*>(groupInfoRes.Error());
      }

      auto groupInfo   = groupInfoRes.Leak().Leak();
      auto newBlockRes = ext2_alloc_block(ctx, groupInfo->groupDesc);

      if (newBlockRes.HasError()) {
        mm_free_ptr(doublePtr);
        mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
        mm_free_ptr(groupInfo);
        return ErrorOr<Void*>(newBlockRes.Error());
      }

      singleIndirectBlock = newBlockRes.Leak().Leak();
      doublePtr[firstIdx] = singleIndirectBlock;

      // Write back GDT
      UInt32 gdtLba = ext2_block_to_lba(ctx, groupInfo->groupDescriptorBlock);
      if (!ext2_write_block(ctx->drive, gdtLba, groupInfo->blockBuffer, blockSize)) {
        mm_free_ptr(doublePtr);
        mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
        mm_free_ptr(groupInfo);
        return ErrorOr<Void*>(kErrorDisk);
      }

      mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
      mm_free_ptr(groupInfo);

      // Zero single-indirect block
      auto zeroBuf = mm_alloc_ptr(blockSize, true, false);

      if (!zeroBuf) {
        mm_free_ptr(doublePtr);
        return ErrorOr<Void*>(kErrorHeapOutOfMemory);
      }

      rt_zero_memory(zeroBuf, blockSize);

      UInt32 singleLba = ext2_block_to_lba(ctx, singleIndirectBlock);

      if (!ext2_write_block(ctx->drive, singleLba, zeroBuf, blockSize)) {
        mm_free_ptr(zeroBuf);
        mm_free_ptr(doublePtr);
        return ErrorOr<Void*>(kErrorDisk);
      }

      mm_free_ptr(zeroBuf);

      // Write double-indirect back to disk
      UInt32 dblLba = ext2_block_to_lba(ctx, node->inode.fBlock[EXT2_DOUBLE_INDIRECT_INDEX]);

      if (!ext2_write_block(ctx->drive, dblLba, doublePtr, blockSize)) {
        mm_free_ptr(doublePtr);
        return ErrorOr<Void*>(kErrorDisk);
      }
    }

    mm_free_ptr(doublePtr);

    // Write to single-indirect block
    auto singleRes = ext2_read_block_ptr(ctx, singleIndirectBlock);
    if (singleRes.HasError()) return ErrorOr<Void*>(singleRes.Error());

    UInt32* singlePtr    = singleRes.Leak().Leak();
    singlePtr[secondIdx] = physicalBlockNumber;

    UInt32 singleLba = ext2_block_to_lba(ctx, singleIndirectBlock);
    if (!ext2_write_block(ctx->drive, singleLba, singlePtr, blockSize)) {
      mm_free_ptr(singlePtr);
      return ErrorOr<Void*>(kErrorDisk);
    }

    mm_free_ptr(singlePtr);
    return ErrorOr<Void*>(nullptr);
  } else if (logicalBlockIndex < doubleStart + tripleSpan) {
    UInt32 tripleLba = ext2_block_to_lba(ctx, node->inode.fBlock[EXT2_TRIPLE_INDIRECT_INDEX]);
    auto triplePtrRef = ext2_read_block_ptr(ctx, node->inode.fBlock[EXT2_TRIPLE_INDIRECT_INDEX]);

    if (triplePtrRef.HasError()) {
      return ErrorOr<Void*>(kErrorDisk);
    }

    if (!ext2_write_block(ctx->drive, tripleLba, triplePtrRef.Leak().Leak(), blockSize)) {
      mm_free_ptr(triplePtrRef.Leak().Leak());
      return ErrorOr<Void*>(kErrorDisk);
    }

    mm_free_ptr(triplePtrRef.Leak().Leak());
  }


  return ErrorOr<Void*>(kErrorDisk);
}

// Find a directory entry by name within a directory inode
static ErrorOr<EXT2_DIR_ENTRY*> ext2_find_dir_entry(Ext2Context* ctx, Ext2Node* dirNode,
                                                    const char* name) {
  if (!ctx || !ctx->drive || !dirNode || !name) return ErrorOr<EXT2_DIR_ENTRY*>(kErrorInvalidData);

  // Check directory type
  auto type = (dirNode->inode.fMode >> 12) & 0xF;
  if (type != kExt2FileTypeDirectory) return ErrorOr<EXT2_DIR_ENTRY*>(kErrorInvalidData);

  UInt32 blockSize = ctx->BlockSize();
  auto   blockBuf  = mm_alloc_ptr(blockSize, true, false);
  if (!blockBuf) return ErrorOr<EXT2_DIR_ENTRY*>(kErrorHeapOutOfMemory);

  SizeT nameLen = rt_string_len(name);
  for (UInt32 i = 0; i < EXT2_DIRECT_BLOCKS; ++i) {
    UInt32 blockNum = dirNode->inode.fBlock[i];
    if (blockNum == 0) continue;

    UInt32 lba = ext2_block_to_lba(ctx, blockNum);
    if (!ext2_read_block(ctx->drive, lba, blockBuf, blockSize)) {
      mm_free_ptr(blockBuf);
      return ErrorOr<EXT2_DIR_ENTRY*>(kErrorDisk);
    }

    UInt32 offset = 0;
    while (offset + sizeof(UInt32) + sizeof(UInt16) <= blockSize) {
      auto onDiskEntry = reinterpret_cast<EXT2_DIR_ENTRY*>((UInt8*) blockBuf + offset);
      if (onDiskEntry->fRecordLength == 0) break;  // corrupted

      if (onDiskEntry->fInode != 0 && onDiskEntry->fNameLength == nameLen) {
        // Compare names
        if (rt_string_cmp(name, onDiskEntry->fName, nameLen) == 0) {
          // Allocate a result sized to hold the name + metadata
          SizeT recSize = sizeof(EXT2_DIR_ENTRY);
          auto  found   = (EXT2_DIR_ENTRY*) mm_alloc_ptr(recSize, true, false);
          if (!found) {
            mm_free_ptr(blockBuf);
            return ErrorOr<EXT2_DIR_ENTRY*>(kErrorHeapOutOfMemory);
          }

          // Copy only record-length bytes
          rt_copy_memory_safe(onDiskEntry, found, onDiskEntry->fRecordLength, recSize);
          mm_free_ptr(blockBuf);
          return ErrorOr<EXT2_DIR_ENTRY*>(found);
        }
      }
      offset += onDiskEntry->fRecordLength;
    }
  }

  mm_free_ptr(blockBuf);
  return ErrorOr<EXT2_DIR_ENTRY*>(kErrorFileNotFound);
}

// Compute ideal record length for a directory name
static inline UInt16 ext2_dir_entry_ideal_len(UInt8 nameLen) {
  UInt16 raw =
      static_cast<UInt16>(8 + nameLen);         // 8 = inode(4)+rec_len(2)+name_len(1)+file_type(1)
  return static_cast<UInt16>((raw + 3) & ~3u);  // align up to 4
}

static ErrorOr<Void*> ext2_add_dir_entry(Ext2Context* ctx, Ext2Node* parentDirNode,
                                         const char* name, UInt32 inodeNumber, UInt8 fileType) {
  using namespace Kernel;

  if (!ctx || !ctx->drive || !parentDirNode || !name) return ErrorOr<Void*>(kErrorInvalidData);

  UInt32 blockSize = ctx->BlockSize();
  SizeT  nameLen   = rt_string_len(name);
  if (nameLen == 0 || nameLen > 255) return ErrorOr<Void*>(kErrorInvalidData);

  UInt16 newRecIdeal = ext2_dir_entry_ideal_len(static_cast<UInt8>(nameLen));

  auto blockBuf = mm_alloc_ptr(blockSize, true, false);
  if (!blockBuf) return ErrorOr<Void*>(kErrorHeapOutOfMemory);

  for (UInt32 bi = 0; bi < EXT2_DIRECT_BLOCKS; ++bi) {
    UInt32 blockNum = parentDirNode->inode.fBlock[bi];

    if (blockNum == 0) {
      // Allocate new block
      auto groupInfoRes = ext2_get_group_descriptor_info(ctx, parentDirNode->inodeNumber);
      if (!groupInfoRes) {
        mm_free_ptr(blockBuf);
        return ErrorOr<Void*>(groupInfoRes.Error());
      }

      auto groupInfo     = *groupInfoRes.Leak();  // Dereference to get ::Detail::Ext2GroupInfo*
      auto allocBlockRes = ext2_alloc_block(ctx, groupInfo->groupDesc);
      if (!allocBlockRes) {
        mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
        mm_free_ptr(groupInfo);
        mm_free_ptr(blockBuf);
        return ErrorOr<Void*>(allocBlockRes.Error());
      }

      UInt32 newBlock = *allocBlockRes.Leak();  // Dereference to get UInt32
      UInt32 gdtLba   = ext2_block_to_lba(ctx, groupInfo->groupDescriptorBlock);

      if (!ext2_write_block(ctx->drive, gdtLba, groupInfo->blockBuffer, blockSize)) {
        mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
        mm_free_ptr(groupInfo);
        mm_free_ptr(blockBuf);
        return ErrorOr<Void*>(kErrorDisk);
      }

      mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
      mm_free_ptr(groupInfo);

      // Zero block & insert entry
      rt_zero_memory(blockBuf, blockSize);
      auto entry           = reinterpret_cast<EXT2_DIR_ENTRY*>(blockBuf);
      entry->fInode        = inodeNumber;
      entry->fNameLength   = static_cast<UInt8>(nameLen);
      entry->fFileType     = fileType;
      entry->fRecordLength = static_cast<UInt16>(blockSize);
      rt_copy_memory_safe(const_cast<char*>(name), entry->fName, nameLen, blockSize);

      UInt32 blockLba = ext2_block_to_lba(ctx, newBlock);
      if (!ext2_write_block(ctx->drive, blockLba, blockBuf, blockSize)) {
        mm_free_ptr(blockBuf);
        return ErrorOr<Void*>(kErrorDisk);
      }

      auto setRes = ext2_set_block_address(ctx, parentDirNode, bi, newBlock);
      if (!setRes) {
        mm_free_ptr(blockBuf);
        return ErrorOr<Void*>(setRes.Error());
      }

      mm_free_ptr(blockBuf);
      return ErrorOr<Void*>(nullptr);
    }

    // read it
    UInt32 blockLba = ext2_block_to_lba(ctx, blockNum);
    if (!ext2_read_block(ctx->drive, blockLba, blockBuf, blockSize)) {
      mm_free_ptr(blockBuf);
      return ErrorOr<Void*>(kErrorDisk);
    }

    UInt32          offset     = 0;
    EXT2_DIR_ENTRY* lastEntry  = nullptr;
    UInt32          lastOffset = 0;

    while (offset < blockSize) {
      if (offset + 8 > blockSize) break;
      auto e = reinterpret_cast<EXT2_DIR_ENTRY*>((UInt8*) blockBuf + offset);
      if (e->fRecordLength == 0) break;
      lastEntry  = e;
      lastOffset = offset;
      offset += e->fRecordLength;
    }

    if (!lastEntry) continue;

    UInt16 lastIdeal = ext2_dir_entry_ideal_len(lastEntry->fNameLength);

    if (lastEntry->fRecordLength >= (UInt16) (lastIdeal + newRecIdeal)) {
      UInt16 origRec           = lastEntry->fRecordLength;
      lastEntry->fRecordLength = lastIdeal;

      UInt32 newOffset        = lastOffset + lastIdeal;
      auto   newEntry         = reinterpret_cast<EXT2_DIR_ENTRY*>((UInt8*) blockBuf + newOffset);
      newEntry->fInode        = inodeNumber;
      newEntry->fNameLength   = static_cast<UInt8>(nameLen);
      newEntry->fFileType     = fileType;
      newEntry->fRecordLength = static_cast<UInt16>(origRec - lastIdeal);
      rt_copy_memory_safe(const_cast<char*>(name), newEntry->fName, nameLen,
                          newEntry->fRecordLength);

      if (!ext2_write_block(ctx->drive, blockLba, blockBuf, blockSize)) {
        mm_free_ptr(blockBuf);
        return ErrorOr<Void*>(kErrorDisk);
      }

      mm_free_ptr(blockBuf);
      return ErrorOr<Void*>(nullptr);
    }
  }

  // No space in direct blocks -> allocate new block
  int targetIndex = -1;
  for (UInt32 i = 0; i < EXT2_DIRECT_BLOCKS; ++i) {
    if (parentDirNode->inode.fBlock[i] == 0) {
      targetIndex = i;
      break;
    }
  }
  if (targetIndex == -1) {
    mm_free_ptr(blockBuf);
    return ErrorOr<Void*>(kErrorUnimplemented);
  }

  auto groupInfoResult = ext2_get_group_descriptor_info(ctx, parentDirNode->inodeNumber);
  if (!groupInfoResult) {
    mm_free_ptr(blockBuf);
    return ErrorOr<Void*>(groupInfoResult.Error());
  }

  auto groupInfo   = *groupInfoResult.Leak();  // Dereference to get ::Detail::Ext2GroupInfo*
  auto newBlockRes = ext2_alloc_block(ctx, groupInfo->groupDesc);
  if (!newBlockRes) {
    mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
    mm_free_ptr(groupInfo);
    mm_free_ptr(blockBuf);
    return ErrorOr<Void*>(newBlockRes.Error());
  }

  UInt32 newBlockNum = *newBlockRes.Leak();  // Dereference to get UInt32
  UInt32 gdtLba      = ext2_block_to_lba(ctx, groupInfo->groupDescriptorBlock);
  if (!ext2_write_block(ctx->drive, gdtLba, groupInfo->blockBuffer, blockSize)) {
    mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
    mm_free_ptr(groupInfo);
    mm_free_ptr(blockBuf);
    return ErrorOr<Void*>(kErrorDisk);
  }

  mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
  mm_free_ptr(groupInfo);

  rt_zero_memory(blockBuf, blockSize);
  auto entry           = reinterpret_cast<EXT2_DIR_ENTRY*>(blockBuf);
  entry->fInode        = inodeNumber;
  entry->fNameLength   = static_cast<UInt8>(nameLen);
  entry->fFileType     = fileType;
  entry->fRecordLength = static_cast<UInt16>(blockSize);
  rt_copy_memory_safe(const_cast<char*>(name), entry->fName, nameLen, blockSize);

  UInt32 newBlockLba = ext2_block_to_lba(ctx, newBlockNum);
  if (!ext2_write_block(ctx->drive, newBlockLba, blockBuf, blockSize)) {
    mm_free_ptr(blockBuf);
    return ErrorOr<Void*>(kErrorDisk);
  }

  auto setRes = ext2_set_block_address(ctx, parentDirNode, targetIndex, newBlockNum);
  if (!setRes) {
    mm_free_ptr(blockBuf);
    return ErrorOr<Void*>(setRes.Error());
  }

  mm_free_ptr(blockBuf);
  return ErrorOr<Void*>(nullptr);
}

// Soon
static ErrorOr<UInt32> ext2_alloc_inode(Ext2Context* ctx, EXT2_GROUP_DESCRIPTOR* groupDesc) {
  if (!ctx || !ctx->superblock || !groupDesc) return ErrorOr<UInt32>(kErrorInvalidData);

  UInt32 blockSize = ctx->BlockSize();

  // buffer for the inode bitmap
  auto bitmap = mm_alloc_ptr(blockSize, true, false);
  if (!bitmap) return ErrorOr<UInt32>(kErrorHeapOutOfMemory);

  // Read inode bitmap
  if (!ext2_read_block(ctx->drive, groupDesc->fInodeBitmap, bitmap, blockSize)) {
    mm_free_ptr(bitmap);
    return ErrorOr<UInt32>(kErrorDisk);
  }

  // Find first free inode (bit = 0)
  for (UInt32 byteIdx = 0; byteIdx < blockSize; ++byteIdx) {
    auto byte = reinterpret_cast<unsigned char*>(bitmap)[byteIdx];
    if (byte != 0xFF) {
      for (int bit = 0; bit < 8; ++bit) {
        if (!(byte & (1 << bit))) {
          // Mark bit as used
          reinterpret_cast<unsigned char*>(bitmap)[byteIdx] |= (1 << bit);

          // Compute inode number
          UInt32 inodeNumber = byteIdx * 8 + bit + 1;  // Inodes are 1-based

          // Write bitmap back
          if (!ext2_write_block(ctx->drive, groupDesc->fInodeBitmap, bitmap, blockSize)) {
            mm_free_ptr(bitmap);
            return ErrorOr<UInt32>(kErrorDisk);
          }

          // Update group descriptor free count
          groupDesc->fFreeInodesCount--;
          mm_free_ptr(bitmap);
          return ErrorOr<UInt32>(inodeNumber);
        }
      }
    }
  }

  mm_free_ptr(bitmap);
  return ErrorOr<UInt32>(kErrorDiskIsFull);
}

// to write an inode to its correct location on disk
static ErrorOr<Void*> ext2_write_inode(Ext2Context* ctx, Ext2Node* node) {
  using namespace Kernel;

  if (!ctx || !ctx->superblock || !ctx->drive || !node) return ErrorOr<Void*>(kErrorInvalidData);

  auto   blockSize      = ctx->BlockSize();
  UInt32 inodesPerGroup = ctx->superblock->fInodesPerGroup;

  if (inodesPerGroup == 0) return ErrorOr<Void*>(kErrorInvalidData);

  // Calculate which group this inode belongs to
  UInt32 groupIndex = (node->inodeNumber - 1) / inodesPerGroup;
  NE_UNUSED(groupIndex);
  UInt32 inodeIndexInGroup = (node->inodeNumber - 1) % inodesPerGroup;

  // Get group descriptor
  auto groupInfoResult = ext2_get_group_descriptor_info(ctx, node->inodeNumber);
  if (!groupInfoResult) return ErrorOr<Void*>(groupInfoResult.Error());

  auto groupInfo = *groupInfoResult.Leak();  // Dereference to get ::Detail::Ext2GroupInfo*

  // Calculate inode table position
  UInt32 inodeTableBlock = groupInfo->groupDesc->fInodeTable;
  UInt32 inodeSize       = ctx->superblock->fInodeSize;
  UInt32 inodesPerBlock  = blockSize / inodeSize;

  UInt32 blockOffset   = inodeIndexInGroup / inodesPerBlock;
  UInt32 offsetInBlock = (inodeIndexInGroup % inodesPerBlock) * inodeSize;

  UInt32 inodeBlock = inodeTableBlock + blockOffset;
  UInt32 inodeLba   = ext2_block_to_lba(ctx, inodeBlock);

  // Read the block containing the inode
  auto blockBuf = mm_alloc_ptr(blockSize, true, false);
  if (!blockBuf) {
    mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
    mm_free_ptr(groupInfo);
    return ErrorOr<Void*>(kErrorHeapOutOfMemory);
  }

  if (!ext2_read_block(ctx->drive, inodeLba, blockBuf, blockSize)) {
    mm_free_ptr(blockBuf);
    mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
    mm_free_ptr(groupInfo);
    return ErrorOr<Void*>(kErrorDisk);
  }

  // Copy the updated inode into the block buffer
  rt_copy_memory_safe(&node->inode, static_cast<void*>((UInt8*) blockBuf + offsetInBlock),
                      sizeof(EXT2_INODE), blockSize - offsetInBlock);

  // Write the block back
  if (!ext2_write_block(ctx->drive, inodeLba, blockBuf, blockSize)) {
    mm_free_ptr(blockBuf);
    mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
    mm_free_ptr(groupInfo);
    return ErrorOr<Void*>(kErrorDisk);
  }

  mm_free_ptr(blockBuf);
  mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
  mm_free_ptr(groupInfo);

  return ErrorOr<Void*>(nullptr);
}

namespace {
// new
struct PathComponents {
  const char** components;
  int          count;
  Char*        buffer;

  PathComponents(const char* path) : components(nullptr), count(0), buffer(nullptr) {
    if (!path || *path == '\0') return;

    SizeT pathLen = rt_string_len(path);
    buffer        = (Char*) mm_alloc_ptr(pathLen + 1, true, false);
    if (!buffer) return;

    rt_copy_memory_safe((void*) path, buffer, pathLen, pathLen + 1);
    buffer[pathLen] = '\0';

    // temp array
    const char** temp = (const char**) mm_alloc_ptr(sizeof(char*) * (pathLen + 1), true, false);
    if (!temp) {
      mm_free_ptr(buffer);
      buffer = nullptr;
      return;
    }

    UInt32 compCount = 0;
    Char*  p         = buffer;

    while (*p != '\0') {
      // skip slashes
      while (*p == '/') p++;
      if (*p == '\0') break;

      Char* start = p;
      while (*p != '/' && *p != '\0') p++;
      Char saved = *p;
      *p         = '\0';

      // handle ".", "..", or normal
      if (rt_string_cmp(start, ".", 1) == 0) {
        // ignore
      } else if (rt_string_cmp(start, "..", 2) == 0) {
        if (compCount > 0) compCount--;  // go up one level
      } else {
        temp[compCount++] = start;
      }

      *p = saved;
    }

    if (compCount == 0) {
      mm_free_ptr(temp);
      return;
    }

    components = (const char**) mm_alloc_ptr(sizeof(char*) * compCount, true, false);
    if (!components) {
      mm_free_ptr(temp);
      return;
    }

    for (UInt32 i = 0; i < compCount; i++) components[i] = temp[i];
    count = compCount;

    mm_free_ptr(temp);
  }

  ~PathComponents() {
    if (components) mm_free_ptr(components);
    if (buffer) mm_free_ptr(buffer);
  }
};
}  // anonymous namespace

// The Ext2FileSystemParser (not manager!)
Ext2FileSystemParser::Ext2FileSystemParser(DriveTrait* drive) : fCtx(drive) {
  MUST_PASS(fCtx);
}

NodePtr Ext2FileSystemParser::Open(const char* path, const char* restrict_type) {
  NE_UNUSED(restrict_type);
  if (!path || *path == '\0' || !this->fCtx.drive) {
    return nullptr;
  }

  // Root ("/")
  if (rt_string_len(path) == 1 && rt_string_cmp(path, "/", 1) == 0) {
    auto inodeResult = ext2_load_inode(&this->fCtx, EXT2_ROOT_INODE);
    if (!inodeResult) {
      return nullptr;
    }

    auto heapNode = (Ext2Node*) mm_alloc_ptr(sizeof(Ext2Node), true, false);
    if (!heapNode) return nullptr;

    *heapNode        = *inodeResult.Leak().Leak();
    heapNode->cursor = 0;
    return reinterpret_cast<NodePtr>(heapNode);
  }

  PathComponents pathComponents(path);
  if (pathComponents.count == 0) {
    return nullptr;
  }

  UInt32    currentInodeNumber = EXT2_ROOT_INODE;
  Ext2Node* currentDirNode     = nullptr;

  for (UInt32 i = 0; i < (UInt32) pathComponents.count; ++i) {
    auto inodeResult = ext2_load_inode(&this->fCtx, currentInodeNumber);
    if (!inodeResult) {
      if (currentDirNode) mm_free_ptr(currentDirNode);
      return nullptr;
    }

    if (currentDirNode) {
      mm_free_ptr(currentDirNode);
      currentDirNode = nullptr;
    }

    currentDirNode = (Ext2Node*) mm_alloc_ptr(sizeof(Ext2Node), true, false);
    if (!currentDirNode) {
      return nullptr;
    }

    *currentDirNode        = *inodeResult.Leak().Leak();
    currentDirNode->cursor = 0;

    if (i < pathComponents.count - 1U) {
      UInt32 type = (currentDirNode->inode.fMode >> 12) & 0xF;
      if (type != kExt2FileTypeDirectory) {
        mm_free_ptr(currentDirNode);
        return nullptr;
      }
    }

    auto dirEntryResult =
        ext2_find_dir_entry(&this->fCtx, currentDirNode, pathComponents.components[i]);
    if (!dirEntryResult) {
      mm_free_ptr(currentDirNode);
      return nullptr;
    }

    EXT2_DIR_ENTRY* entryPtr = *dirEntryResult.Leak();
    currentInodeNumber       = entryPtr->fInode;
    mm_free_ptr(entryPtr);
  }

  auto finalInodeResult = ext2_load_inode(&this->fCtx, currentInodeNumber);
  if (!finalInodeResult) {
    if (currentDirNode) mm_free_ptr(currentDirNode);
    return nullptr;
  }

  if (currentDirNode) {
    mm_free_ptr(currentDirNode);
  }

  auto resultNode = (Ext2Node*) mm_alloc_ptr(sizeof(Ext2Node), true, false);
  if (!resultNode) {
    return nullptr;
  }

  *resultNode        = *finalInodeResult.Leak().Leak();
  resultNode->cursor = 0;
  return reinterpret_cast<NodePtr>(resultNode);
}

void* Ext2FileSystemParser::Read(NodePtr node, Int32 flags, SizeT size) {
  if (!node) return nullptr;

  NE_UNUSED(flags);

  auto extNode    = reinterpret_cast<Ext2Node*>(node);
  auto dataResult = ext2_read_inode_data(&this->fCtx, extNode, size);

  if (!dataResult) {
    return nullptr;  // error, nothing to return
  }

  void* data = *dataResult.Leak();
  if (data) {
    extNode->cursor += static_cast<UInt32>(size);
  }

  return data;
}

void Ext2FileSystemParser::Write(NodePtr node, void* data, Int32 flags, SizeT size) {
  if (!node || !data || size == 0) return;

  NE_UNUSED(flags);

  auto  extNode      = reinterpret_cast<Ext2Node*>(node);
  auto  blockSize    = this->fCtx.BlockSize();
  SizeT bytesWritten = 0;

  UInt32 currentOffset = extNode->cursor;
  UInt8* src           = reinterpret_cast<UInt8*>(data);

  while (bytesWritten < size) {
    UInt32 logicalBlockIndex = currentOffset / blockSize;
    UInt32 offsetInBlock     = currentOffset % blockSize;

    auto   physBlockResult = ext2_get_block_address(&this->fCtx, extNode, logicalBlockIndex);
    UInt32 physicalBlock   = 0;

    if (!physBlockResult) {
      auto err = physBlockResult.Error();
      if (err == kErrorInvalidData || err == kErrorUnimplemented) {
        auto groupInfoResult = ext2_get_group_descriptor_info(&this->fCtx, extNode->inodeNumber);
        if (!groupInfoResult) {
          return;
        }

        auto groupInfo   = *groupInfoResult.Leak();
        auto allocResult = ext2_alloc_block(&this->fCtx, groupInfo->groupDesc);
        if (!allocResult) {
          mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
          mm_free_ptr(groupInfo);
          return;
        }

        physicalBlock = *allocResult.Leak();

        auto setRes =
            ext2_set_block_address(&this->fCtx, extNode, logicalBlockIndex, physicalBlock);
        if (!setRes) {
          mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
          mm_free_ptr(groupInfo);
          return;
        }

        UInt32 gdtLba = ext2_block_to_lba(&this->fCtx, groupInfo->groupDescriptorBlock);
        if (!ext2_write_block(this->fCtx.drive, gdtLba, groupInfo->blockBuffer, blockSize)) {
          mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
          mm_free_ptr(groupInfo);
          return;
        }

        mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
        mm_free_ptr(groupInfo);
      } else {
        return;
      }
    } else {
      physicalBlock = physBlockResult.Value();
    }

    UInt32 physicalLba = ext2_block_to_lba(&this->fCtx, physicalBlock);

    auto blockBuf = mm_alloc_ptr(blockSize, true, false);
    if (!blockBuf) return;

    if (offsetInBlock > 0 || (size - bytesWritten) < blockSize) {
      if (!ext2_read_block(this->fCtx.drive, physicalLba, blockBuf, blockSize)) {
        mm_free_ptr(blockBuf);
        return;
      }
    } else {
      rt_zero_memory(blockBuf, blockSize);
    }

    UInt32 bytesInCurrentBlock =
        static_cast<UInt32>(::Detail::ext2_min(size - bytesWritten, blockSize - offsetInBlock));
    rt_copy_memory_safe(src, static_cast<void*>((UInt8*) blockBuf + offsetInBlock),
                        bytesInCurrentBlock, blockSize - offsetInBlock);

    if (!ext2_write_block(this->fCtx.drive, physicalLba, blockBuf, blockSize)) {
      mm_free_ptr(blockBuf);
      return;
    }

    mm_free_ptr(blockBuf);

    currentOffset += bytesInCurrentBlock;
    src += bytesInCurrentBlock;
    bytesWritten += bytesInCurrentBlock;
  }

  if (currentOffset > extNode->inode.fSize) {
    extNode->inode.fSize = currentOffset;
  }

  extNode->inode.fBlocks     = (extNode->inode.fSize + blockSize - 1) / blockSize;
  extNode->inode.fModifyTime = 0;

  auto writeInodeRes = ext2_write_inode(&this->fCtx, extNode);
  if (!writeInodeRes) {
    // Failed to persist inode
  }

  extNode->cursor = currentOffset;
}

bool Ext2FileSystemParser::Seek(NodePtr node, SizeT offset) {
  if (!node) return false;
  auto extNode    = reinterpret_cast<Ext2Node*>(node);
  extNode->cursor = static_cast<UInt32>(offset);
  return true;
}

SizeT Ext2FileSystemParser::Tell(NodePtr node) {
  if (!node) return 0;
  auto extNode = reinterpret_cast<Ext2Node*>(node);
  return extNode->cursor;
}

bool Ext2FileSystemParser::Rewind(NodePtr node) {
  if (!node) return false;
  auto extNode    = reinterpret_cast<Ext2Node*>(node);
  extNode->cursor = 0;
  return true;
}

void* Ext2FileSystemParser::Read(const char* name, NodePtr node, Int32 flags, SizeT size) {
  NE_UNUSED(name);
  return Read(node, flags, size);
}

void Ext2FileSystemParser::Write(const char* name, NodePtr node, void* data, Int32 flags,
                                 SizeT size) {
  NE_UNUSED(name);
  Write(node, data, flags, size);
}

NodePtr Ext2FileSystemParser::Create(const char* path) {
  if (!path || *path == '\0') return nullptr;

  PathComponents pathComponents(path);
  if (pathComponents.count == 0) return nullptr;

  const char* filename = pathComponents.components[pathComponents.count - 1];
  if (rt_string_len(filename) > kExt2FSMaxFileNameLen) return nullptr;

  // Build parent path
  Char  parentPathBuf[256] = {0};
  SizeT currentPathLen     = 0;
  for (UInt32 i = 0; (i < pathComponents.count - 1U); ++i) {
    SizeT componentLen = rt_string_len(pathComponents.components[i]);
    if (currentPathLen + componentLen + 1 >= sizeof(parentPathBuf)) return nullptr;
    if (i > 0) parentPathBuf[currentPathLen++] = '/';
    rt_copy_memory_safe(const_cast<char*>(pathComponents.components[i]),
                        parentPathBuf + currentPathLen, componentLen,
                        sizeof(parentPathBuf) - currentPathLen);
    currentPathLen += componentLen;
  }
  parentPathBuf[currentPathLen] = '\0';

  // Open parent directory
  NodePtr parentDirNodePtr = nullptr;
  if (currentPathLen == 0) {
    // root
    auto inodeRes = ext2_load_inode(&this->fCtx, EXT2_ROOT_INODE);
    if (!inodeRes) return nullptr;
    parentDirNodePtr = mm_alloc_ptr(sizeof(Ext2Node), true, false);
    if (!parentDirNodePtr) return nullptr;
    *reinterpret_cast<Ext2Node*>(parentDirNodePtr)        = *inodeRes.Leak().Leak();
    reinterpret_cast<Ext2Node*>(parentDirNodePtr)->cursor = 0;
  } else {
    parentDirNodePtr = Open(parentPathBuf, "r");
  }

  if (!parentDirNodePtr) return nullptr;

  auto parentDirNode = reinterpret_cast<Ext2Node*>(parentDirNodePtr);

  // Ensure parent is a directory
  UInt32 type = (parentDirNode->inode.fMode >> 12) & 0xF;
  if (type != kExt2FileTypeDirectory) {
    mm_free_ptr(parentDirNode);
    return nullptr;
  }

  // Get group info for allocation
  auto groupInfoResult = ext2_get_group_descriptor_info(&this->fCtx, parentDirNode->inodeNumber);
  if (!groupInfoResult) {
    mm_free_ptr(parentDirNode);
    return nullptr;
  }
  auto groupInfo = *groupInfoResult.Leak();

  // Allocate new inode
  auto newInodeRes = ext2_alloc_inode(&this->fCtx, groupInfo->groupDesc);
  if (!newInodeRes) {
    mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
    mm_free_ptr(groupInfo);  // so this works
    mm_free_ptr(parentDirNode);
    return nullptr;
  }
  UInt32 newInodeNumber = newInodeRes.Value();

  UInt32 gdtLba = ext2_block_to_lba(&this->fCtx, groupInfo->groupDescriptorBlock);
  if (!ext2_write_block(this->fCtx.drive, gdtLba, groupInfo->blockBuffer, this->fCtx.BlockSize())) {
    mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
    mm_free_ptr(groupInfo);
    mm_free_ptr(parentDirNode);
    return nullptr;
  }

  mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
  mm_free_ptr(groupInfo);

  // Create new Ext2Node
  Ext2Node* newFileNode = reinterpret_cast<Ext2Node*>(mm_alloc_ptr(sizeof(Ext2Node), true, false));
  if (!newFileNode) {
    mm_free_ptr(parentDirNode);
    return nullptr;
  }

  newFileNode->inodeNumber = newInodeNumber;
  rt_zero_memory(&newFileNode->inode, sizeof(EXT2_INODE));

  newFileNode->inode.fMode       = (kExt2FileTypeRegular << 12);
  newFileNode->inode.fUID        = 0;
  newFileNode->inode.fGID        = 0;
  newFileNode->inode.fLinksCount = 1;
  newFileNode->inode.fSize       = 0;
  newFileNode->inode.fBlocks     = 0;
  newFileNode->inode.fCreateTime = 0;
  newFileNode->inode.fModifyTime = 0;

  // Persist new inode
  auto writeInodeRes = ext2_write_inode(&this->fCtx, newFileNode);
  if (!writeInodeRes) {
    mm_free_ptr(parentDirNode);
    mm_free_ptr(newFileNode);
    return nullptr;
  }

  // Add directory entry
  auto addRes = ext2_add_dir_entry(&this->fCtx, parentDirNode, filename, newInodeNumber,
                                   kExt2FileTypeRegular);
  if (!addRes) {
    mm_free_ptr(parentDirNode);
    mm_free_ptr(newFileNode);
    return nullptr;
  }

  // Update parent inode
  auto parentWriteRes = ext2_write_inode(&this->fCtx, parentDirNode);
  // ignore failure

  NE_UNUSED(parentWriteRes);

  mm_free_ptr(parentDirNode);
  return reinterpret_cast<NodePtr>(newFileNode);
}

NodePtr Ext2FileSystemParser::CreateDirectory(const char* path) {
  if (!path || *path == '\0') return nullptr;

  PathComponents pathComponents(path);
  if (pathComponents.count == 0) {
    kout << "EXT2: Failed to parse path for CreateDirectory.\n";
    return nullptr;
  }

  const char* dirname = pathComponents.components[pathComponents.count - 1];
  if (rt_string_len(dirname) > kExt2FSMaxFileNameLen) {
    kout << "EXT2: Directory name too long: " << dirname << ".\n";
    return nullptr;
  }

  // Build parent path
  Char  parentPathBuf[256];
  SizeT currentPathLen = 0;
  for (UInt32 i = 0; (i < pathComponents.count - 1U); ++i) {
    SizeT componentLen = rt_string_len(pathComponents.components[i]);
    if (currentPathLen + componentLen + 1 >= sizeof(parentPathBuf)) {
      kout << "EXT2: Parent path too long for CreateDirectory.\n";
      return nullptr;
    }

    if (i > 0) parentPathBuf[currentPathLen++] = '/';

    rt_copy_memory_safe(static_cast<void*>(const_cast<char*>(pathComponents.components[i])),
                        static_cast<void*>(parentPathBuf + currentPathLen), componentLen,
                        sizeof(parentPathBuf) - currentPathLen);
    currentPathLen += componentLen;
  }

  parentPathBuf[currentPathLen] = '\0';

  // Open parent directory node
  NodePtr parentDirNodePtr = nullptr;
  if (currentPathLen == 0) {
    auto inodeRes = ext2_load_inode(&this->fCtx, EXT2_ROOT_INODE);
    if (!inodeRes) {
      return nullptr;
    }

    parentDirNodePtr = reinterpret_cast<NodePtr>(mm_alloc_ptr(sizeof(Ext2Node), true, false));
    if (!parentDirNodePtr) return nullptr;

    *reinterpret_cast<Ext2Node*>(parentDirNodePtr)        = *inodeRes.Leak().Leak();
    reinterpret_cast<Ext2Node*>(parentDirNodePtr)->cursor = 0;
  } else {
    parentDirNodePtr = Open(parentPathBuf, "r");
  }

  if (!parentDirNodePtr) {
    kout << "EXT2: Failed to open parent directory for CreateDirectory: " << parentPathBuf << ".\n";
    return nullptr;
  }

  auto parentDirNode = reinterpret_cast<Ext2Node*>(parentDirNodePtr);

  // Check parent is a directory
  UInt32 parentType = (parentDirNode->inode.fMode >> 12) & 0xF;
  if (parentType != kExt2FileTypeDirectory) {
    kout << "EXT2: Parent is not a directory: " << parentPathBuf << ".\n";
    mm_free_ptr(parentDirNode);
    return nullptr;
  }

  // Allocate inode
  auto groupInfoResult = ext2_get_group_descriptor_info(&this->fCtx, parentDirNode->inodeNumber);
  if (!groupInfoResult) {
    kout << "EXT2: Failed to get group descriptor info for new dir inode.\n";
    mm_free_ptr(parentDirNode);
    return nullptr;
  }

  auto groupInfo   = *groupInfoResult.Leak();
  auto newInodeRes = ext2_alloc_inode(&this->fCtx, groupInfo->groupDesc);
  if (!newInodeRes) {
    kout << "EXT2: Failed to allocate inode for new directory.\n";
    mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
    mm_free_ptr(groupInfo);
    mm_free_ptr(parentDirNode);
    return nullptr;
  }

  UInt32 newInodeNumber = *newInodeRes.Leak();

  // Write back group descriptor block
  UInt32 gdtLba = ext2_block_to_lba(&this->fCtx, groupInfo->groupDescriptorBlock);
  if (!ext2_write_block(this->fCtx.drive, gdtLba, groupInfo->blockBuffer, this->fCtx.BlockSize())) {
    kout << "EXT2: Failed to write group descriptor after inode allocation.\n";
    mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
    mm_free_ptr(groupInfo);
    mm_free_ptr(parentDirNode);
    return nullptr;
  }

  mm_free_ptr(reinterpret_cast<void*>(groupInfo->blockBuffer));
  mm_free_ptr(groupInfo);

  // Create new Ext2Node and initialize inode fields
  Ext2Node* newDirNode = reinterpret_cast<Ext2Node*>(mm_alloc_ptr(sizeof(Ext2Node), true, false));
  if (!newDirNode) {
    kout << "EXT2: Out of memory for new directory node.\n";
    mm_free_ptr(parentDirNode);
    return nullptr;
  }

  newDirNode->inodeNumber = newInodeNumber;
  rt_zero_memory(&newDirNode->inode, sizeof(EXT2_INODE));
  newDirNode->inode.fMode       = (kExt2FileTypeDirectory << 12);
  newDirNode->inode.fUID        = 0;
  newDirNode->inode.fGID        = 0;
  newDirNode->inode.fLinksCount = 2;  // . and ..
  newDirNode->inode.fSize       = this->fCtx.BlockSize();
  newDirNode->inode.fBlocks     = 1;
  newDirNode->inode.fCreateTime = 0;
  newDirNode->inode.fModifyTime = 0;

  // Allocate a data block for the new directory
  auto groupForBlockRes = ext2_get_group_descriptor_info(&this->fCtx, newDirNode->inodeNumber);
  if (!groupForBlockRes) {
    kout << "EXT2: Failed to get group info for directory block allocation.\n";
    mm_free_ptr(parentDirNode);
    mm_free_ptr(newDirNode);
    return nullptr;
  }

  auto groupForBlock = *groupForBlockRes.Leak();
  auto newBlockRes   = ext2_alloc_block(&this->fCtx, groupForBlock->groupDesc);
  if (!newBlockRes) {
    kout << "EXT2: Failed to allocate block for new directory contents.\n";
    mm_free_ptr(reinterpret_cast<void*>(groupForBlock->blockBuffer));
    mm_free_ptr(groupForBlock);
    mm_free_ptr(parentDirNode);
    mm_free_ptr(newDirNode);
    return nullptr;
  }

  UInt32 newDirBlockNum = *newBlockRes.Leak();

  // Write back GDT
  UInt32 gdtLba2 = ext2_block_to_lba(&this->fCtx, groupForBlock->groupDescriptorBlock);
  if (!ext2_write_block(this->fCtx.drive, gdtLba2, groupForBlock->blockBuffer,
                        this->fCtx.BlockSize())) {
    kout << "EXT2: Failed to write GDT after directory block allocation.\n";
    mm_free_ptr(reinterpret_cast<void*>(groupForBlock->blockBuffer));
    mm_free_ptr(groupForBlock);
    mm_free_ptr(parentDirNode);
    mm_free_ptr(newDirNode);
    return nullptr;
  }

  mm_free_ptr(reinterpret_cast<void*>(groupForBlock->blockBuffer));
  mm_free_ptr(groupForBlock);

  // Set the block in newDirNode
  auto setBlkRes = ext2_set_block_address(&this->fCtx, newDirNode, 0, newDirBlockNum);
  if (!setBlkRes) {
    kout << "EXT2: Failed to set data block for new directory.\n";
    mm_free_ptr(parentDirNode);
    mm_free_ptr(newDirNode);
    return nullptr;
  }

  // Prepare block with '.' and '..'
  auto dirBlockBuf = mm_alloc_ptr(this->fCtx.BlockSize(), true, false);
  if (!dirBlockBuf) {
    kout << "EXT2: Out of memory preparing directory block.\n";
    mm_free_ptr(parentDirNode);
    mm_free_ptr(newDirNode);
    return nullptr;
  }

  rt_zero_memory(dirBlockBuf, this->fCtx.BlockSize());

  // '.' entry
  auto dot           = reinterpret_cast<EXT2_DIR_ENTRY*>(dirBlockBuf);
  dot->fInode        = newInodeNumber;
  dot->fNameLength   = 1;
  dot->fFileType     = kExt2FileTypeDirectory;
  dot->fRecordLength = ext2_dir_entry_ideal_len(dot->fNameLength);
  dot->fName[0]      = '.';

  // '..' entry occupies rest of block
  auto dotdot    = reinterpret_cast<EXT2_DIR_ENTRY*>((UInt8*) dirBlockBuf + dot->fRecordLength);
  dotdot->fInode = parentDirNode->inodeNumber;
  dotdot->fNameLength   = 2;
  dotdot->fFileType     = kExt2FileTypeDirectory;
  dotdot->fRecordLength = static_cast<UInt16>(this->fCtx.BlockSize() - dot->fRecordLength);
  dotdot->fName[0]      = '.';
  dotdot->fName[1]      = '.';

  // Write dir block to disk
  UInt32 newDirBlockLba = ext2_block_to_lba(&this->fCtx, newDirBlockNum);
  if (!ext2_write_block(this->fCtx.drive, newDirBlockLba, dirBlockBuf, this->fCtx.BlockSize())) {
    kout << "EXT2: Failed to write directory block to disk.\n";
    mm_free_ptr(dirBlockBuf);
    mm_free_ptr(parentDirNode);
    mm_free_ptr(newDirNode);
    return nullptr;
  }

  mm_free_ptr(dirBlockBuf);

  // Persist new directory inode
  auto writeInodeRes = ext2_write_inode(&this->fCtx, newDirNode);
  if (!writeInodeRes) {
    kout << "EXT2: Failed to write new directory inode to disk.\n";
    mm_free_ptr(parentDirNode);
    mm_free_ptr(newDirNode);
    return nullptr;
  }

  // Add directory entry into parent
  auto addRes = ext2_add_dir_entry(&this->fCtx, parentDirNode, dirname, newInodeNumber,
                                   kExt2FileTypeDirectory);
  if (!addRes) {
    kout << "EXT2: Failed to add directory entry for '" << dirname << "' to parent.\n";
    mm_free_ptr(parentDirNode);
    mm_free_ptr(newDirNode);
    return nullptr;
  }

  // Increment parent link count and persist parent inode
  parentDirNode->inode.fLinksCount += 1;
  auto parentWriteRes = ext2_write_inode(&this->fCtx, parentDirNode);
  if (!parentWriteRes) {
    kout << "EXT2: Warning: failed to update parent inode after directory creation.\n";
  }

  mm_free_ptr(parentDirNode);
  return reinterpret_cast<NodePtr>(newDirNode);
}

#endif
#endif
