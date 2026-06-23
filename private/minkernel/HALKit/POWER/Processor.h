// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef HALKIT_PROCESSOR_H
#define HALKIT_PROCESSOR_H

#include <NeKit/Config.h>
#include <NeKit/Utils.h>

#define rtl_nop_op() asm volatile("mr 0, 0")
#define kHalPPCAlignment __attribute__((aligned(4)))

namespace Ne::Kernel::HAL {
typedef UIntPtr Reg;

/// @brief Stack frame (as retrieved from assembly.)
struct PACKED StackFrame final {
  Reg R8{0};
  Reg R9{0};
  Reg R10{0};
  Reg R11{0};
  Reg R12{0};
  Reg R13{0};
  Reg R14{0};
  Reg R15{0};
  Reg SP{0};
  Reg IP{0};
};

typedef StackFrame* StackFramePtr;

inline void rt_halt() {
  while (true) {
    NoOp();  // no oop.
  }
}

inline void rt_cli() {
  NoOp();  // no oop
}
}  // namespace Ne::Kernel::HAL

EXTERN_C Ne::Kernel::Void int_handle_math(Ne::Kernel::UIntPtr sp);
EXTERN_C Ne::Kernel::Void int_handle_pf(Ne::Kernel::UIntPtr sp);

/// @brief Set TLB.
Ne::Kernel::Bool hal_set_tlb(Ne::Kernel::UInt8 tlb, Ne::Kernel::UInt32 epn, Ne::Kernel::UInt64 rpn,
                         Ne::Kernel::UInt8 perms, Ne::Kernel::UInt8 wimge, Ne::Kernel::UInt8 ts,
                         Ne::Kernel::UInt8 esel, Ne::Kernel::UInt8 tsize, Ne::Kernel::UInt8 iprot);

/// @brief Write TLB.
Ne::Kernel::Void hal_write_tlb(Ne::Kernel::UInt32 mas0, Ne::Kernel::UInt32 mas1, Ne::Kernel::UInt32 mas2,
                           Ne::Kernel::UInt32 mas3, Ne::Kernel::UInt32 mas7);

/// @brief Flush TLB.
EXTERN_C Ne::Kernel::Void hal_flush_tlb();

#endif
