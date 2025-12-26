/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

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
  Bool    fUndefined{YES};  // is this instance undefined?
  KString fKey;
  KString fValue;
};
}  // namespace Kernel

#endif