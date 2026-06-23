// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef HALKIT_HYPERVISOR_H
#define HALKIT_HYPERVISOR_H

#include <NeKit/Config.h>

namespace Ne::Kernel {
MAKE_STRING_ENUM(HYPERVISOR)
ENUM_STRING(Qemu, "TCGTCGTCGTCG");
ENUM_STRING(KVM, " KVMKVMKVM  ");
ENUM_STRING(VMWare, "VMwareVMware");
ENUM_STRING(VirtualBox, "VBoxVBoxVBox");
ENUM_STRING(Xen, "XenVMMXenVMM");
ENUM_STRING(Microsoft, "Microsoft Hv");
ENUM_STRING(Parallels, " prl hyperv ");
ENUM_STRING(ParallelsAlt, " lrpepyh vr ");
ENUM_STRING(Bhyve, "bhyve bhyve ");
ENUM_STRING(Qnx, " QNXQVMBSQG ");
END_STRING_ENUM()
}  // namespace Ne::Kernel

#endif
