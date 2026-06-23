/* ========================================

Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license..

File: LTE\LTE.h.
Purpose: LTE Standard Library.

======================================== */

#ifndef _INC_NETWORK_LTE_H_
#define _INC_NETWORK_LTE_H_

#include <NeKit/Config.h>
#include <NeKit/KString.h>

/// @brief Long Term Evolution I/O routines.

/// @brief Turn on SIM slot.
Ne::Kernel::Boolean lte_turn_on_sim(Ne::Kernel::Int32 simSlot);

/// @brief Turn off SIM slot.
Ne::Kernel::Boolean lte_turn_off_sim(Ne::Kernel::Int32 simSlot);

/// @brief Send AT command.
Ne::Kernel::Boolean lte_send_at_command(Ne::Kernel::Char* buf, Ne::Kernel::Size bufReadSz,
                                    Ne::Kernel::Int32 simSlot);

Ne::Kernel::Boolean lte_write_sim_file(Ne::Kernel::Char* file, Ne::Kernel::VoidPtr buf, Ne::Kernel::Size bufSz,
                                   Ne::Kernel::Size offset, Ne::Kernel::Int32 simSlot);

Ne::Kernel::VoidPtr lte_read_sim_file(Ne::Kernel::Char* file, Ne::Kernel::Size bufSz, Ne::Kernel::Size offset,
                                  Ne::Kernel::Int32 simSlot);

#endif  // ifndef _INC_NETWORK_LTE_H_
