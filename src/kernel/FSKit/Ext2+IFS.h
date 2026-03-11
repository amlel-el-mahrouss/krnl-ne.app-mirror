// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel


#ifndef FSKIT_EXT2_IFS_H
#define FSKIT_EXT2_IFS_H

#include <FSKit/Ext2.h>
#include <KernelKit/DriveMgr.h>
#include <KernelKit/HeapMgr.h>
#include <KernelKit/KPC.h>
#include <NeKit/ErrorOr.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/Utils.h>

namespace Kernel {
/// @brief Context for an EXT2 filesystem on a given drive
class Ext2Context final {
 public:
  DriveTrait*       drive{nullptr};
  EXT2_SUPER_BLOCK* superblock{nullptr};

  /// @brief context with a drive
  Ext2Context(Kernel::DriveTrait* drv) : drive(drv) {}

  /// @brief Clean up
  ~Ext2Context() {
    if (superblock) {
      Kernel::mm_free_ptr(superblock);
      superblock = nullptr;
    }
  }

  Ext2Context(const Ext2Context&)            = delete;
  Ext2Context& operator=(const Ext2Context&) = delete;

  Ext2Context(Ext2Context&& other) : drive(other.drive), superblock(other.superblock) {
    other.drive      = nullptr;
    other.superblock = nullptr;
  }

  Ext2Context& operator=(Ext2Context&& other) {
    if (this != &other) {
      if (superblock) {
        Kernel::mm_free_ptr(superblock);
      }
      drive            = other.drive;
      superblock       = other.superblock;
      other.drive      = nullptr;
      other.superblock = nullptr;
    }
    return *this;
  }

  SizeT BlockSize() const {
    if (!superblock) return kExt2FSBlockSizeBase;
    return kExt2FSBlockSizeBase << superblock->fLogBlockSize;
  }

  operator BOOL() { return superblock != nullptr; }
};

/// ======================================================================= ///
/// IFS FUNCTIONS
/// ======================================================================= ///

inline BOOL ext2_read_block(Kernel::DriveTrait* drv, Kernel::UInt32 lba, VoidPtr buffer,
                            Kernel::UInt32 size) {
  if (!drv || !buffer) return false;

  Kernel::DriveTrait::DrivePacket pkt{};
  pkt.fPacketContent = buffer;
  pkt.fPacketSize    = size;
  pkt.fPacketLba     = lba;
  drv->fInput(pkt);

  return pkt.fPacketGood;
}

inline BOOL ext2_write_block(Kernel::DriveTrait* drv, Kernel::UInt32 lba, const VoidPtr buffer,
                             Kernel::UInt32 size) {
  if (!drv || !buffer) return false;

  Kernel::DriveTrait::DrivePacket pkt{};
  pkt.fPacketContent = const_cast<VoidPtr>(buffer);
  pkt.fPacketSize    = size;
  pkt.fPacketLba     = lba;
  drv->fOutput(pkt);
  return pkt.fPacketGood;
}

inline Kernel::ErrorOr<EXT2_SUPER_BLOCK*> ext2_load_superblock(Ext2Context* ctx) {
  if (!ctx || !ctx->drive) return Kernel::ErrorOr<EXT2_SUPER_BLOCK*>(Kernel::kErrorInvalidData);

  auto buf = Kernel::mm_alloc_ptr(sizeof(EXT2_SUPER_BLOCK), true, false);
  if (!buf) return Kernel::ErrorOr<EXT2_SUPER_BLOCK*>(Kernel::kErrorHeapOutOfMemory);

  Kernel::UInt32 blockLba = kExt2FSSuperblockOffset / ctx->drive->fSectorSz;

  if (!ext2_read_block(ctx->drive, blockLba, buf, sizeof(EXT2_SUPER_BLOCK))) {
    Kernel::mm_free_ptr(buf);
    return Kernel::ErrorOr<EXT2_SUPER_BLOCK*>(Kernel::kErrorDisk);
  }

  auto sb = reinterpret_cast<EXT2_SUPER_BLOCK*>(buf);
  if (sb->fMagic != kExt2FSMagic) {
    Kernel::mm_free_ptr(buf);
    return Kernel::ErrorOr<EXT2_SUPER_BLOCK*>(Kernel::kErrorInvalidData);
  }

  ctx->superblock = sb;
  return Kernel::ErrorOr<EXT2_SUPER_BLOCK*>(sb);
}

// Load inode
inline Kernel::ErrorOr<Ext2Node*> ext2_load_inode(Ext2Context* ctx, Kernel::UInt32 inodeNumber) {
  if (!ctx || !ctx->superblock) return Kernel::ErrorOr<Ext2Node*>(Kernel::kErrorInvalidData);

  auto nodePtr = Kernel::mm_alloc_ptr(sizeof(Ext2Node), true, false);
  if (!nodePtr) return Kernel::ErrorOr<Ext2Node*>(Kernel::kErrorHeapOutOfMemory);

  auto ext2Node         = reinterpret_cast<Ext2Node*>(nodePtr);
  ext2Node->inodeNumber = inodeNumber;

  // Compute block group and index within group
  Kernel::UInt32 inodesPerGroup = ctx->superblock->fInodesPerGroup;
  Kernel::UInt32 group          = (inodeNumber - 1) / inodesPerGroup;

  // dummy: just offset first inode
  Kernel::UInt32 inodeTableBlock = ctx->superblock->fFirstInode + group;

  if (!ext2_read_block(ctx->drive, inodeTableBlock, &ext2Node->inode, sizeof(EXT2_INODE))) {
    Kernel::mm_free_ptr(nodePtr);
    return Kernel::ErrorOr<Ext2Node*>(Kernel::kErrorDisk);
  }

  ext2Node->cursor = 0;
  return Kernel::ErrorOr<Ext2Node*>(ext2Node);
}

/*
 * Ext2FileSystemParser Class
 *
 * Provides high-level interface for EXT2 filesystem operations
 */
class Ext2FileSystemParser final {
 private:
  Ext2Context fCtx;  // Internal EXT2 context

 public:
  /*
   * Constructor
   * Initializes the parser with a drive interface
   *
   * @param drive: Pointer to drive trait for disk I/O operations
   */
  explicit Ext2FileSystemParser(DriveTrait* drive);

  NE_COPY_DELETE(Ext2FileSystemParser)

  /*
   * Open a file or directory by path
   *
   * @param path: Full path to the file/directory (e.g., "/home/user/file.txt")
   * @param restrict_type: Access mode restriction (e.g., "r", "w", "rw")
   * @return: VoidPtr handle to the opened file/directory, or nullptr on failure
   */
  VoidPtr Open(const char* path, const char* restrict_type);

  /*
   * Read data from an open file node
   *
   * @param node: File node handle returned by Open()
   * @param flags: Read operation flags
   * @param size: Number of bytes to read
   * @return: Pointer to allocated buffer containing read data, or nullptr on failure
   *          Caller is responsible for freeing the returned buffer
   */
  VoidPtr Read(VoidPtr node, Int32 flags, SizeT size);

  /*
   * Write data to an open file node
   *
   * @param node: File node handle returned by Open()
   * @param data: Buffer containing data to write
   * @param flags: Write operation flags
   * @param size: Number of bytes to write
   */
  Void Write(VoidPtr node, VoidPtr data, Int32 flags, SizeT size);

  /*
   * Seek to a specific position in the file
   *
   * @param node: File node handle
   * @param offset: Byte offset from beginning of file
   * @return: true on success, false on failure
   */
  BOOL Seek(VoidPtr node, SizeT offset);

  /*
   * Get current position in the file
   *
   * @param node: File node handle
   * @return: Current byte offset from beginning of file
   */
  SizeT Tell(VoidPtr node);

  /*
   * Reset file position to beginning
   *
   * @param node: File node handle
   * @return: true on success, false on failure
   */
  BOOL Rewind(VoidPtr node);

  /*
   * Read data from a named file within a directory node
   *
   * @param name: Name of file within the directory
   * @param node: Directory node handle
   * @param flags: Read operation flags
   * @param size: Number of bytes to read
   * @return: Pointer to allocated buffer containing read data, or nullptr on failure
   */
  VoidPtr Read(const char* name, VoidPtr node, Int32 flags, SizeT size);

  /*
   * Write data to a named file within a directory node
   *
   * @param name: Name of file within the directory
   * @param node: Directory node handle
   * @param data: Buffer containing data to write
   * @param flags: Write operation flags
   * @param size: Number of bytes to write
   */
  Void Write(const char* name, VoidPtr node, VoidPtr data, Int32 flags, SizeT size);

  /*
   * Create a new regular file
   *
   * @param path: Full path for the new file
   * @return: VoidPtr handle to the created file, or nullptr on failure
   */
  VoidPtr Create(const char* path);

  /*
   * Create a new directory
   *
   * @param path: Full path for the new directory
   * @return: VoidPtr handle to the created directory, or nullptr on failure
   */
  VoidPtr CreateDirectory(const char* path);

  /*
   * Close and free a file/directory node
   * Note: This method is not shown in the implementation but would typically be needed
   *
   * @param node: Node handle to close and free
   */
  Void Close(VoidPtr node);

  /*
   * Get file/directory information
   * Note: This method is not shown in the implementation but would typically be needed
   *
   * @param node: Node handle
   * @param info: Structure to fill with file information (size, type, permissions, etc.)
   * @return: true on success, false on failure
   */
  BOOL GetInfo(VoidPtr node, VoidPtr info);
};
}  // namespace Kernel

#endif
