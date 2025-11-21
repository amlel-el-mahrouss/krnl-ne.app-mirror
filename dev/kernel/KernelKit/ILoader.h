/* ========================================

  Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#pragma once

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Defines.h>
#include <NeKit/ErrorOr.h>
#include <hint/CompilerHint.h>

namespace Kernel {
/// @brief This interface is used to make loader contracts (MSCOFF, PEF).
/// @author @Amlal-El-Mahrouss
class ILoader {
 public:
  explicit ILoader() = default;
  virtual ~ILoader() = default;

  NE_COPY_DEFAULT(ILoader)

 public:
  virtual _Output ErrorOr<VoidPtr> GetBlob()                                              = 0;
  virtual _Output const Char* AsString()                                                  = 0;
  virtual _Output const Char* MIME()                                                      = 0;
  virtual _Output const Char* Path()                                                      = 0;
  virtual _Output ErrorOr<VoidPtr> FindStart()                                            = 0;
  virtual _Output ErrorOr<VoidPtr> FindSymbol(_Input const Char* name, _Input Int32 kind) = 0;
};
}  // namespace Kernel
