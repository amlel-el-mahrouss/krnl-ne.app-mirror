/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#include <NeKit/Variant.h>

namespace Kernel {
const Char* Variant::ToString() {
  switch (fKind) {
    case VariantKind::kTOML:
      return ("Class:{TOML}");
    case VariantKind::kJson:
      return ("Class:{Json}");
    case VariantKind::kString:
      return ("Class:{String}");
    case VariantKind::kBlob:
      return ("Class:{Blob}");
    case VariantKind::kNull:
      return ("Class:{Null}");
    case VariantKind::kSwap:
      return ("Class:{Swap}");
    default:
      return ("Class:{Unknown}");
  }
}

/// @brief Return variant's kind.
Variant::VariantKind& Variant::Kind() {
  return this->fKind;
}

/// @brief Leak variant's instance.
RefAny Variant::Leak() {
  return {this->fPtr};
}
}  // namespace Kernel
