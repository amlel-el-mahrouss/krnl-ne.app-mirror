// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#include <KernelKit/KPC.h>
#include <KernelKit/ProcessScheduler.h>
#include <NetworkKit/IPC.h>

namespace Ne::Kernel {
bool IPC_ADDR::operator==(const IPC_ADDR& addr) {
  return addr.UserProcessID == this->UserProcessID && addr.UserProcessTeam == this->UserProcessTeam;
}

bool IPC_ADDR::operator==(IPC_ADDR& addr) {
  return addr.UserProcessID == this->UserProcessID && addr.UserProcessTeam == this->UserProcessTeam;
}

bool IPC_ADDR::operator!=(const IPC_ADDR& addr) {
  return addr.UserProcessID != this->UserProcessID || addr.UserProcessTeam != this->UserProcessTeam;
}

bool IPC_ADDR::operator!=(IPC_ADDR& addr) {
  return addr.UserProcessID != this->UserProcessID || addr.UserProcessTeam != this->UserProcessTeam;
}
}  // namespace Ne::Kernel
