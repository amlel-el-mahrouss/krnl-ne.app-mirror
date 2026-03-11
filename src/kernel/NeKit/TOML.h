// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef NEKIT_TOML_H
#define NEKIT_TOML_H

#include <NeKit/Config.h>
#include <NeKit/KString.h>

namespace Kernel {

/// \brief TOML Object Handle
class TOMLObject final {
 public:
  explicit TOMLObject() = delete;
  ~TOMLObject()         = default;

  STATIC TOMLObject kNull;

 private:
  Bool    mUndefined{YES};  // is this instance undefined?
  KString mKey;
  KString mValue;
};

}  // namespace Kernel

#endif
