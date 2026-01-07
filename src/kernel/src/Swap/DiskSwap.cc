// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#include <KernelKit/FileMgr.h>
#include <SwapKit/DiskSwap.h>

namespace Kernel {
static constexpr UInt32 kSwapDiskHeaderMagic = 0x44535750;  // 'DSWP'

/***********************************************************************************/
/// @brief Write memory chunk onto disk.
/// @param data the data packet.
/// @return Whether the swap was written to disk, or not.
/***********************************************************************************/
BOOL IDiskSwap::Write(SwapDiskHdr* data) {
  if (!data || data->fMagic != kSwapDiskHeaderMagic) return NO;

  FileStream file(kSwapPageFilePath, kRestrictWRB);

  ErrorOr<Int64> ret = file.Write(data->fOffset, data, sizeof(SwapDiskHdr) + data->fBlobSz);

  return ret.Value() < kErrorSuccess;
}

/***********************************************************************************/
/// @brief Read memory chunk from disk.
/// @param data the data packet length.
/// @return Whether the swap was fetched to disk, or not.
/***********************************************************************************/
SwapDiskHdr* IDiskSwap::Read(const UIntPtr offset, SizeT data_len) {
  if (data_len > kSwapBlockMaxSize) return nullptr;
  if (data_len == 0) return nullptr;

  FileStream file(kSwapPageFilePath, kRestrictRB);

  VoidPtr blob = file.Read(offset, sizeof(SwapDiskHdr) + data_len);

  if (!blob || (static_cast<SwapDiskHdr*>(blob))->fMagic != kSwapDiskHeaderMagic) {
    if (blob) mm_free_ptr(blob);
    return nullptr;
  }

  return reinterpret_cast<SwapDiskHdr*>(blob);
}
}  // namespace Kernel
