// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel
#ifndef NEKIT_PAGEMGR_H
#define NEKIT_PAGEMGR_H

#include <NeKit/Config.h>
#include <NeKit/Ref.h>

namespace Kernel {
class PageMgr;

class PTEWrapper final {
 public:
  explicit PTEWrapper(Boolean Rw = false, Boolean User = false, Boolean ExecDisable = false,
                      UIntPtr Address = 0);

  ~PTEWrapper();

  PTEWrapper& operator=(const PTEWrapper&) = default;
  PTEWrapper(const PTEWrapper&)            = default;

 public:
  UIntPtr VirtualAddress();

  Void NoExecute(const bool enable = false);
  Bool NoExecute();

  operator bool() { return fVirtAddr; }

  bool Reclaim();
  bool Shareable();
  bool Present();
  bool Access();

 private:
  Boolean fRw;
  Boolean fUser;
  Boolean fExecDisable;
  UIntPtr fVirtAddr;
  Boolean fCache;
  Boolean fShareable;
  Boolean fWt;
  Boolean fPresent;
  Boolean fAccessed;

 private:
  friend class PageMgr;
  friend class Pmm;
};

struct PageMgr final {
 public:
  PageMgr()  = default;
  ~PageMgr() = default;

  PageMgr& operator=(const PageMgr&) = default;
  PageMgr(const PageMgr&)            = default;

 public:
  PTEWrapper Request(Boolean Rw, Boolean User, Boolean ExecDisable, SizeT Sz, SizeT Pad);
  bool       Free(Ref<PTEWrapper>& wrapper);

 private:
  void FlushTLB();

 private:
  friend PTEWrapper;
  friend class Pmm;
};
}  // namespace Kernel

#endif
