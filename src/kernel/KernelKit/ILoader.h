// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef KERNELKIT_ILOADER_H
#define KERNELKIT_ILOADER_H

#include <CompilerKit/CompilerKit.h>
#include <NeKit/Config.h>
#include <NeKit/ErrorOr.h>
#include <hint/CompilerHint.h>

#define NE_EXEC_LOADER : public ::Kernel::ILoader

namespace Kernel {

/// @brief The ILoader interface is used to make dynamic loader contracts (such as: MSCOFF, PEF).
/// @author Amlal El Mahrouss
class ILoader {
 public:
  explicit ILoader() = default;
  virtual ~ILoader() = default;

  NE_COPY_DEFAULT(ILoader)

 public:
  virtual _Output ErrorOr<VoidPtr> GetBlob() { return ErrorOr<VoidPtr>{}; }
  virtual _Output const Char* AsString() { return "(null)"; }
  virtual _Output const Char* MIME() { return "*/*"; }
  virtual _Output const Char* Path() { return "(null)"; }
  virtual _Output ErrorOr<VoidPtr> FindStart() { return ErrorOr<VoidPtr>{}; }
  virtual _Output ErrorOr<VoidPtr> FindSymbol(_Input const Char*, _Input Int32) {
    return ErrorOr<VoidPtr>{};
  }
};

}  // namespace Kernel

#endif
