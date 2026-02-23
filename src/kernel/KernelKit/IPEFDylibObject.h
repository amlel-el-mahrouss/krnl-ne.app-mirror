// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef KERNELKIT_PEF_SHARED_OBJECT_H
#define KERNELKIT_PEF_SHARED_OBJECT_H

#include <KernelKit/IDylibObject.h>
#include <KernelKit/PEF.h>
#include <KernelKit/PEFCodeMgr.h>
#include <KernelKit/ProcessScheduler.h>
#include <NeKit/Config.h>

namespace Kernel {

/**
 * @brief Shared Library class
 * Load library from this class
 */
class IPEFDylibObject final NE_DYLIB_OBJECT {
 public:
  explicit IPEFDylibObject() = default;
  ~IPEFDylibObject()         = default;

 public:
  NE_COPY_DEFAULT(IPEFDylibObject)

  using DylibTraitsPtr   = DylibTraits*;
  using DylibTraitsPtrX2 = DylibTraitsPtr*;

 private:
  DylibTraitsPtr fMounted{nullptr};

 public:
  DylibTraitsPtrX2 GetAddressOf() { return &fMounted; }

  DylibTraitsPtr Get() { return fMounted; }

 public:
  void Mount(DylibTraitsPtr to_mount) {
    if (!to_mount) return;
    if (!to_mount->ImageObject) return;

    fMounted = to_mount;

    if (fLoader && to_mount) {
      delete fLoader;
      fLoader = nullptr;
    }

    if (!fLoader) {
      fLoader = new PEFLoader(fMounted->ImageObject);
    }
  }

  void Unmount() {
    if (fMounted) fMounted = nullptr;
  };

  template <typename SymbolType>
  SymbolType Load(const Char* symbol_name, const SizeT& len, const UInt32& kind) {
    if (symbol_name == nullptr || *symbol_name == 0) return nullptr;
    if (len > kPathLen || len < 1) return nullptr;

    auto ret = static_cast<SymbolType>(fLoader->FindSymbol(symbol_name, kind).Leak().Leak());

    if (!ret) {
      if (kind == kPefCode) return (VoidPtr) &__ne_pure_call;

      return nullptr;
    }

    return ret;
  }

 private:
  using LoaderType = PEFLoader;

  LoaderType* fLoader{nullptr};
};

using IDylibRef = IPEFDylibObject*;

EXTERN_C IDylibRef rtl_init_dylib_pef(UserProcess& header);
EXTERN_C Void      rtl_fini_dylib_pef(UserProcess& header, IDylibRef lib, Bool* successful);

}  // namespace Kernel

#endif /* ifndef __KERNELKIT_PEF_SHARED_OBJECT_H__ */
