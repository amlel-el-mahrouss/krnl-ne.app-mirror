
/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

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
