// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#ifndef BOOTKIT_BOOTTHREAD_H
#define BOOTKIT_BOOTTHREAD_H

#include <FirmwareKit/Handover.h>
#include <KernelKit/MSDOS.h>
#include <KernelKit/PE.h>

namespace Boot {
using namespace Kernel;

class BootThread;

/// @brief Bootloader Thread class.
class BootThread final {
 public:
  explicit BootThread() = delete;
  ~BootThread()         = default;

  explicit BootThread(Kernel::VoidPtr blob);

  BootThread& operator=(const BootThread&) = default;
  BootThread(const BootThread&)            = default;

  Int32       Start(HEL::BootInfoHeader* handover, BOOL is_own_stack);
  void        SetName(const char* name);
  const char* GetName();
  bool        IsValid();

 private:
  Char                 fBlobName[256U] = {"BootThread"};
  VoidPtr              fStartAddress{nullptr};
  VoidPtr              fBlob{nullptr};
  UInt8*               fStack{nullptr};
  HEL::BootInfoHeader* fHandover{nullptr};
};
}  // namespace Boot

#endif
