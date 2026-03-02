// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <CompilerKit/CompilerKit.h>
#include <FSKit/IndexableProperty.h>
#include <NeKit/KString.h>
#include <NeKit/KernelPanic.h>
#include <NeKit/MutableArray.h>
#include <NeKit/Utils.h>

/// @brief File indexer API for fast path access.
/// BUGS: 0

#define kMaxLenIndexer (256U)

namespace Kernel {
namespace Indexer {
  Index& IndexableProperty::Leak() {
    return fIndex;
  }

  Void IndexableProperty::AddFlag(UInt16 flag) {
    fFlags |= flag;
  }

  Void IndexableProperty::RemoveFlag(UInt16 flag) {
    fFlags &= ~(flag);
  }

  UInt16 IndexableProperty::HasFlag(UInt16 flag) {
    return fFlags & flag;
  }

  /// @brief Index a file into the indexer instance.
  /// @param filename filesystem path to access.
  /// @param filenameLen used bytes in path.
  /// @param indexer the filesystem indexer.
  /// @return none, check before if indexer can be claimed (using indexer.HasFlag(kIndexerClaimed)).
  Void fs_index_file(const Char* filename, SizeT filenameLen, IndexableProperty& indexer) {
    if (!indexer.HasFlag(kIndexerClaimed)) {
      indexer.RemoveFlag(kIndexerUnclaimed);
      indexer.AddFlag(kIndexerClaimed);
      rt_copy_memory_safe(reinterpret_cast<VoidPtr>(const_cast<Char*>(filename)),
                          (VoidPtr) indexer.Leak().Path, filenameLen, kIndexerCatalogNameLength);

      (Void)(kout << "FSKit: Indexed new file: " << filename << kendl);
    }
  }
}  // namespace Indexer
}  // namespace Kernel
