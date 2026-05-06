// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef NEKIT_PMM_H
#define NEKIT_PMM_H

#include <NeKit/Config.h>
#include <NeKit/PageMgr.h>
#include <NeKit/Ref.h>

namespace Kernel {
class Pmm;
class PTEWrapper;

// TODO: A PMM stands for Page Memory Manager. Which guarantees page retrieval and free.
class Pmm final {
 public:
  explicit Pmm();
  ~Pmm();

  Pmm& operator=(const Pmm&) = delete;
  Pmm(const Pmm&)            = default;

  Ref<PTEWrapper> RequestPage(Boolean user = false, Boolean readWrite = false);
  Boolean         FreePage(Ref<PTEWrapper> refPage);

  Boolean ToggleRw(Ref<PTEWrapper> refPage, Boolean enable = true);
  Boolean TogglePresent(Ref<PTEWrapper> refPage, Boolean enable = true);
  Boolean ToggleUser(Ref<PTEWrapper> refPage, Boolean enable = true);
  Boolean ToggleShare(Ref<PTEWrapper> refPage, Boolean enable = true);

  /// @brief Get the page manager of this.
  Ref<PageMgr>& Leak() { return fPageMgr; }

 private:
  Ref<PageMgr> fPageMgr;
};
}  // namespace Kernel

#endif
