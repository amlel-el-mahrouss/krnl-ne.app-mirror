
// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef FW_HEADERS_CF_OBJECT_H
#define FW_HEADERS_CF_OBJECT_H

#include <CoreFoundation/headers/Foundation.h>

#define CF_OBJECT : public CF::CFObject

namespace CF {
class CFObject;

class CFObject {
 public:
  explicit CFObject() = default;
  virtual ~CFObject() = default;

  LIBSYS_COPY_DEFAULT(CFObject);

  virtual const Char* Name() { return "[\"CFObject\"];"; }
};
}  // namespace CF

#endif
