// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel


#ifndef SIGNALKIT_SIGNALS_H
#define SIGNALKIT_SIGNALS_H

#include <NeKit/Config.h>
#include <NeKit/KernelPanic.h>

#define SIGBAD 0  /* bad signal*/
#define SIGKILL 1 /* kill */
#define SIGPAUS 2 /* pause */
#define SIGEXEC 3 /* execute */
#define SIGTRAP 4 /* trap */
#define SIGABRT 5 /* abort */
#define SIGCONT 6 /* continue */
#define SIGSEG 7  /* process fault */
#define SIGBREK 8
#define SIGATCH 9
#define SIGDTCH 10

/// @author Amlal El Mahrouss
/// @brief Signal Generation API.

namespace Kernel {

using rt_signal_kind = SizeT;

/// @brief Standard signal seed for general purpose usage.
inline constexpr auto kUserSignalSeed = 0x0895034fUL;

/// @brief Special signal seed for kernel usage.
inline constexpr auto kKernelSignalSeed = 0x0895034f9fUL;

/// @brief Generate signal from **Sig**
template <rt_signal_kind Sig, SizeT Seed = kUserSignalSeed>
inline rt_signal_kind sig_generate_unique() {
  STATIC_PASS(Sig > SIGBAD, "Signal is zero (invalid)");
  return Sig ^ Seed;
}

/// @brief Checks if the signal matches the seed (user_seed or kernel_seed)
template <SizeT Seed>
inline BOOL sig_matches_seed(const rt_signal_kind& sig) {
  return (sig & 0xFF000000) == (Seed & 0xFF000000);
}

/// @brief Validate signal from **sig** and whtether the signal is greater than SIGDTCH.
inline BOOL sig_validate_unique(const rt_signal_kind& sig) {
  return sig > SIGBAD && sig > SIGDTCH;
}

}  // namespace Kernel

#endif
