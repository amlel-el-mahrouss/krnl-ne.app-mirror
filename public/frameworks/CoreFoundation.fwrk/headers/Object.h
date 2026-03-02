
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef HEADERS_OBJECT_H
#define HEADERS_OBJECT_H

#include <CoreFoundation.fwrk/headers/Foundation.h>

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
