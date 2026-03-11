// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel
#ifndef FSKIT_INDEXABLEPROPERTY_H
#define FSKIT_INDEXABLEPROPERTY_H

#include <CFKit/Property.h>
#include <CompilerKit/CompilerKit.h>
#include <KernelKit/DriveMgr.h>

#define kIndexerCatalogNameLength (256U)
#define kIndexerClaimed (0xCF)
#define kIndexerUnclaimed (0xCA)

namespace Kernel {
namespace Indexer {
  struct Index final {
   public:
    Char Drive[kDriveNameLen]{};
    Char Path[kIndexerCatalogNameLength]{};
  };

  class IndexableProperty final : public Property {
   public:
    explicit IndexableProperty() : Property() {
      KBasicString<> strProp(kMaxPropLen);
      strProp += "/prop/indexable";

      this->GetKey() = strProp;
    }

    ~IndexableProperty() override = default;

    NE_COPY_DEFAULT(IndexableProperty)

   public:
    Index& Leak();

   public:
    Void   AddFlag(UInt16 flag);
    Void   RemoveFlag(UInt16 flag);
    UInt16 HasFlag(UInt16 flag);

   private:
    Index  fIndex;
    UInt32 fFlags{};
  };

  /// @brief Index a file into the indexer instance.
  /// @param filename path
  /// @param filenameLen used bytes in path.
  /// @param indexer the filesystem indexer.
  /// @return none.
  Void fs_index_file(const Char* filename, SizeT filenameLen, IndexableProperty& indexer);
}  // namespace Indexer
}  // namespace Kernel

#endif
