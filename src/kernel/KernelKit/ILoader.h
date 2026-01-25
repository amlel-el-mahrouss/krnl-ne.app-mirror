// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef KERNELKIT_ILOADER_H
#define KERNELKIT_ILOADER_H

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Config.h>
#include <NeKit/ErrorOr.h>
#include <hint/CompilerHint.h>

namespace Kernel {
/// @brief This interface is used to make loader contracts (MSCOFF, PEF).
/// @author Amlal El Mahrouss
class ILoader {
 public:
  explicit ILoader() = default;
  virtual ~ILoader() = default;

  NE_COPY_DEFAULT(ILoader)

 public:
  virtual _Output ErrorOr<VoidPtr> GetBlob() { return ErrorOr<VoidPtr>{}; }
  virtual _Output const Char*      AsString() { return ""; }
  virtual _Output const Char*      MIME() { return ""; }
  virtual _Output const Char*      Path() { return ""; }
  virtual _Output ErrorOr<VoidPtr> FindStart() { return ErrorOr<VoidPtr>{}; }
  virtual _Output ErrorOr<VoidPtr> FindSymbol(_Input const Char*, _Input Int32) {
    return ErrorOr<VoidPtr>{};
  }
};
}  // namespace Kernel

#endif
