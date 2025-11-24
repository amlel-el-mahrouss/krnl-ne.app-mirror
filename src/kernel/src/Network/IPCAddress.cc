/* ========================================

   Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

   ======================================== */

#include <KernelKit/KPC.h>
#include <KernelKit/ProcessScheduler.h>
#include <NetworkKit/IPC.h>

namespace Kernel {
bool IPC_ADDR::operator==(const IPC_ADDR& addr) noexcept {
  return addr.UserProcessID == this->UserProcessID && addr.UserProcessTeam == this->UserProcessTeam;
}

bool IPC_ADDR::operator==(IPC_ADDR& addr) noexcept {
  return addr.UserProcessID == this->UserProcessID && addr.UserProcessTeam == this->UserProcessTeam;
}

bool IPC_ADDR::operator!=(const IPC_ADDR& addr) noexcept {
  return addr.UserProcessID != this->UserProcessID || addr.UserProcessTeam != this->UserProcessTeam;
}

bool IPC_ADDR::operator!=(IPC_ADDR& addr) noexcept {
  return addr.UserProcessID != this->UserProcessID || addr.UserProcessTeam != this->UserProcessTeam;
}
}  // namespace Kernel
