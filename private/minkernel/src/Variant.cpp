// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <NeKit/Variant.h>

namespace Kernel {
KString Variant::ToString() {
  switch (fKind) {
    case VariantKind::kTOML:
      return ("Class:{TOML}");
    case VariantKind::kJson:
      return ("Class:{Json}");
    case VariantKind::kString:
      return ("Class:{String}");
    case VariantKind::kBlob:
      return ("Memory:{Blob}");
    case VariantKind::kNull:
      return ("Memory:{Null}");
    case VariantKind::kSwap:
      return ("Memory:{Swap}");
    default:
      return ("Memory:{Unknown}");
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
