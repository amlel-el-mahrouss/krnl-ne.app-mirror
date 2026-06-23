// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef AHCI_AHCI_H
#define AHCI_AHCI_H

#include <NeKit/Config.h>
#include <modules/ACPI/ACPI.h>

/// @file AHCI.h
/// @brief AHCI support.

#define kAHCISectorSize (512)

struct HbaPort;
struct FisData;
struct FisRegD2H;
struct FisRegH2D;

/// @brief Frame information type.
enum {
  kFISTypeRegH2D   = 0x27,  // Register FIS - host to device
  kFISTypeRegD2H   = 0x34,  // Register FIS - device to host
  kFISTypeDMAAct   = 0x39,  // DMA activate FIS - device to host
  kFISTypeDMASetup = 0x41,  // DMA setup FIS - bidirectional
  kFISTypeData     = 0x46,  // Data FIS - bidirectional
  kFISTypeBIST     = 0x58,  // BIST activate FIS - bidirectional
  kFISTypePIOSetup = 0x5F,  // PIO setup FIS - device to host
  kFISTypeDevBits  = 0xA1,  // Set device bits FIS - device to host
};

enum {
  kAHCICmdIdentify   = 0xEC,
  kAHCICmdReadDma    = 0xC8,
  kAHCICmdReadDmaEx  = 0x25,
  kAHCICmdWriteDma   = 0xCA,
  kAHCICmdWriteDmaEx = 0x35
};

typedef struct FisRegH2D final {
  // DWORD 0
  Ne::Kernel::UInt8 FisType;  // FIS_TYPE_REG_H2D

  Ne::Kernel::UInt8 PortMul : 4;    // Port multiplier
  Ne::Kernel::UInt8 Reserved0 : 3;  // Reserved
  Ne::Kernel::UInt8 CmdOrCtrl : 1;  // 1: Command, 0: Control

  Ne::Kernel::UInt8 Command;     // Command register
  Ne::Kernel::UInt8 FeatureLow;  // Feature register, 7:0

  // DWORD 1
  Ne::Kernel::UInt8 Lba0;    // LBA low register, 7:0
  Ne::Kernel::UInt8 Lba1;    // LBA mid register, 15:8
  Ne::Kernel::UInt8 Lba2;    // LBA high register, 23:16
  Ne::Kernel::UInt8 Device;  // Device register

  // DWORD 2
  Ne::Kernel::UInt8 Lba3;         // LBA register, 31:24
  Ne::Kernel::UInt8 Lba4;         // LBA register, 39:32
  Ne::Kernel::UInt8 Lba5;         // LBA register, 47:40
  Ne::Kernel::UInt8 FeatureHigh;  // Feature register, 15:8

  // DWORD 3
  Ne::Kernel::UInt8 CountLow;   // Count register, 7:0
  Ne::Kernel::UInt8 CountHigh;  // Count register, 15:8
  Ne::Kernel::UInt8 Icc;        // Isochronous command completion
  Ne::Kernel::UInt8 Control;    // Control register

  // DWORD 4
  Ne::Kernel::UInt8 Reserved1[4];  // Reserved
} FisRegH2D;

typedef struct FisRegD2H final {
  // DWORD 0
  Ne::Kernel::UInt8 FisType;  // FIS_TYPE_REG_D2H

  Ne::Kernel::UInt8 PortMul : 4;    // Port multiplier
  Ne::Kernel::UInt8 Reserved0 : 2;  // Reserved
  Ne::Kernel::UInt8 IE : 1;         // Interrupt bit
  Ne::Kernel::UInt8 Reserved1 : 1;  // Reserved

  Ne::Kernel::UInt8 Status;  // Status register
  Ne::Kernel::UInt8 Error;   // Error register

  // DWORD 1
  Ne::Kernel::UInt8 Lba0;    // LBA low register, 7:0
  Ne::Kernel::UInt8 Lba1;    // LBA mid register, 15:8
  Ne::Kernel::UInt8 Lba2;    // LBA high register, 23:16
  Ne::Kernel::UInt8 Device;  // Device register

  // DWORD 2
  Ne::Kernel::UInt8 Lba3;  // LBA register, 31:24
  Ne::Kernel::UInt8 Lba4;  // LBA register, 39:32
  Ne::Kernel::UInt8 Lba5;  // LBA register, 47:40
  Ne::Kernel::UInt8 Rsv2;  // Reserved

  // DWORD 3
  Ne::Kernel::UInt8 CountLow;   // Count register, 7:0
  Ne::Kernel::UInt8 CountHigh;  // Count register, 15:8
  Ne::Kernel::UInt8 Rsv3[2];    // Reserved

  // DWORD 4
  Ne::Kernel::UInt8 Rsv4[4];  // Reserved
} FisRegD2H;

typedef struct FisData final {
  // DWORD 0
  Ne::Kernel::UInt8 FisType;  // FIS_TYPE_DATA

  Ne::Kernel::UInt8 PortMul : 4;    // Port multiplier
  Ne::Kernel::UInt8 Reserved0 : 4;  // Reserved

  Ne::Kernel::UInt8 Reserved1[2];  // Reserved

  // DWORD 1 ~ N
  Ne::Kernel::UInt32 Data[1];  // Payload
} FisData;

typedef struct FisPioSetup final {
  // DWORD 0
  Ne::Kernel::UInt8 FisType;  // FIS_TYPE_PIO_SETUP

  Ne::Kernel::UInt8 PortMul : 4;    // Port multiplier
  Ne::Kernel::UInt8 Reserved0 : 1;  // Reserved
  Ne::Kernel::UInt8 DTD : 1;        // Data transfer direction, 1 - device to host
  Ne::Kernel::UInt8 IE : 1;         // Interrupt bit
  Ne::Kernel::UInt8 Reserved1 : 1;

  Ne::Kernel::UInt8 Status;  // Status register
  Ne::Kernel::UInt8 Error;   // Error register

  // DWORD 1
  Ne::Kernel::UInt8 Lba0;    // LBA low register, 7:0
  Ne::Kernel::UInt8 Lba1;    // LBA mid register, 15:8
  Ne::Kernel::UInt8 Lba2;    // LBA high register, 23:16
  Ne::Kernel::UInt8 Device;  // Device register

  // DWORD 2
  Ne::Kernel::UInt8 Lba3;  // LBA register, 31:24
  Ne::Kernel::UInt8 Lba4;  // LBA register, 39:32
  Ne::Kernel::UInt8 Lba5;  // LBA register, 47:40
  Ne::Kernel::UInt8 Rsv2;  // Reserved

  // DWORD 3
  Ne::Kernel::UInt8 CountLow;   // Count register, 7:0
  Ne::Kernel::UInt8 CountHigh;  // Count register, 15:8
  Ne::Kernel::UInt8 Rsv3;       // Reserved
  Ne::Kernel::UInt8 EStatus;    // New value of status register

  // DWORD 4
  Ne::Kernel::UInt16 TranferCount;  // Transfer count
  Ne::Kernel::UInt8  Rsv4[2];       // Reserved
} FisPioSetup;

typedef struct FisDmaSetup final {
  // DWORD 0
  Ne::Kernel::UInt8 FisType;  // FIS_TYPE_DMA_SETUP

  Ne::Kernel::UInt8 PortMul : 4;     // Port multiplier
  Ne::Kernel::UInt8 Reserved0 : 1;   // Reserved
  Ne::Kernel::UInt8 DTD : 1;         // Data transfer direction, 1 - device to host
  Ne::Kernel::UInt8 IE : 1;          // Interrupt bit
  Ne::Kernel::UInt8 AutoEnable : 1;  // Auto-activate. Specifies if DMA Activate FIS is needed

  Ne::Kernel::UInt8 Reserved1[2];  // Reserved

  // DWORD 1&2
  volatile Ne::Kernel::UInt64 DmaBufferId;  // DMA Buffer Identifier. Used to Identify DMA buffer in
                                        // host memory. SATA Spec says host specific and not in
                                        // Spec. Trying AHCI spec might work.

  // DWORD 3
  Ne::Kernel::UInt32 Rsvd;  // More reserved

  // DWORD 4
  Ne::Kernel::UInt32 DmabufOffset;  // Byte offset into buffer. First 2 bits must be 0

  // DWORD 5
  Ne::Kernel::UInt32 TransferCount;  // Number of bytes to transfer. Bit 0 must be 0

  // DWORD 6
  Ne::Kernel::UInt32 Reserved3;  // Reserved
} FisDmaSetup;

typedef struct FisDevBits final {
  // DWORD 0
  Ne::Kernel::UInt8 FisType;  // FIS_TYPE_DMA_SETUP (A1h)

  Ne::Kernel::UInt8 Reserved0 : 5;  // Reserved
  Ne::Kernel::UInt8 R0 : 1;
  Ne::Kernel::UInt8 IE : 1;
  Ne::Kernel::UInt8 N : 1;

  Ne::Kernel::UInt8 StatusLow : 3;
  Ne::Kernel::UInt8 R1 : 1;
  Ne::Kernel::UInt8 StatusHigh : 3;

  Ne::Kernel::UInt8 R2 : 1;
  Ne::Kernel::UInt8 Error;

  // DWORD 1
  Ne::Kernel::UInt32 Act;
} FisDevBits;

/// \brief Enable AHCI device bit in GHC register.
#ifndef kSATAGHC_AE
#define kSATAGHC_AE (31)
#endif  //! ifndef kSATAGHC_AE

typedef struct HbaPort final {
  Ne::Kernel::UInt32 Clb;            // 0x00, command list base address, 1K-byte aligned
  Ne::Kernel::UInt32 Clbu;           // 0x04, command list base address upper 32 bits
  Ne::Kernel::UInt32 Fb;             // 0x08, FIS base address, 256-byte aligned
  Ne::Kernel::UInt32 Fbu;            // 0x0C, FIS base address upper 32 bits
  Ne::Kernel::UInt32 Is;             // 0x10, interrupt status
  Ne::Kernel::UInt32 Ie;             // 0x14, interrupt enable
  Ne::Kernel::UInt32 Cmd;            // 0x18, command and status
  Ne::Kernel::UInt32 Reserved0;      // 0x1C, Reserved
  Ne::Kernel::UInt32 Tfd;            // 0x20, task file data
  Ne::Kernel::UInt32 Sig;            // 0x24, signature
  Ne::Kernel::UInt32 Ssts;           // 0x28, SATA status (SCR0:SStatus)
  Ne::Kernel::UInt32 Sctl;           // 0x2C, SATA control (SCR2:SControl)
  Ne::Kernel::UInt32 Serr;           // 0x30, SATA error (SCR1:SError)
  Ne::Kernel::UInt32 Sact;           // 0x34, SATA active (SCR3:SActive)
  Ne::Kernel::UInt32 Ci;             // 0x38, command issue
  Ne::Kernel::UInt32 Sntf;           // 0x3C, SATA notification (SCR4:SNotification)
  Ne::Kernel::UInt32 Fbs;            // 0x40, FIS-based switch control
  Ne::Kernel::UInt32 Reserved1[11];  // 0x44 ~ 0x6F, Reserved
  Ne::Kernel::UInt32 Vendor[4];      // 0x70 ~ 0x7F, vendor specific
} HbaPort;

typedef struct HbaMem final {
  // 0x00 - 0x2B, Generic Host Control
  Ne::Kernel::UInt32 Cap;      // 0x00, Host capability
  Ne::Kernel::UInt32 Ghc;      // 0x04, Global host control
  Ne::Kernel::UInt32 Is;       // 0x08, Interrupt status
  Ne::Kernel::UInt32 Pi;       // 0x0C, Port implemented
  Ne::Kernel::UInt32 Vs;       // 0x10, Version
  Ne::Kernel::UInt32 Ccc_ctl;  // 0x14, Command completion coalescing control
  Ne::Kernel::UInt32 Ccc_pts;  // 0x18, Command completion coalescing ports
  Ne::Kernel::UInt32 Em_loc;   // 0x1C, Enclosure management location
  Ne::Kernel::UInt32 Em_ctl;   // 0x20, Enclosure management control
  Ne::Kernel::UInt32 Cap2;     // 0x24, Host capabilities extended
  Ne::Kernel::UInt32 Bohc;     // 0x28, BIOS/OS handoff control and status

  Ne::Kernel::UInt8 Resv0[0xA0 - 0x2C];
  Ne::Kernel::UInt8 Vendor[0x100 - 0xA0];

  HbaPort Ports[1];  // 1 ~ 32, 32 is the max ahci devices per controller.
} HbaMem;

typedef HbaMem* HbaMemRef;

typedef struct HbaCmdHeader final {
  // DW0
  union HbaFlags {
    struct HbaFlags_ {
      Ne::Kernel::UInt8 Cfl : 5;           // Command FIS length in DWORDS, 2 ~ 16
      Ne::Kernel::UInt8 Atapi : 1;         // ATAPI
      Ne::Kernel::UInt8 Write : 1;         // Write, 1: H2D, 0: D2H
      Ne::Kernel::UInt8 Prefetchable : 1;  // Prefetchable

      Ne::Kernel::UInt8 Reset : 1;      // Reset
      Ne::Kernel::UInt8 BIST : 1;       // BIST
      Ne::Kernel::UInt8 Clear : 1;      // Clear busy upon R_OK
      Ne::Kernel::UInt8 Reserved0 : 1;  // Reserved
      Ne::Kernel::UInt8 Pmp : 4;        // Port multiplier port
    } Struct;

    Ne::Kernel::UInt16 Flags;
  } HbaFlags;

  Ne::Kernel::UInt16 Prdtl;  // Physical region descriptor table length in entries
  Ne::Kernel::UInt32 Prdbc;  // Physical region descriptor byte count transferred

  Ne::Kernel::UInt32 Ctba;   // Command table descriptor base address
  Ne::Kernel::UInt32 Ctbau;  // Command table descriptor base address upper 32 bits

  Ne::Kernel::UInt32 Rsv[4];
} ATTRIBUTE(packed, aligned(32)) HbaCmdHeader;

typedef struct HbaFis final {
  // 0x00
  FisDmaSetup   Dsfis;  // DMA Setup FIS
  Ne::Kernel::UInt8 Pad0[4];
  // 0x20
  FisPioSetup   Psfis;  // PIO Setup FIS
  Ne::Kernel::UInt8 Pad1[12];
  // 0x40
  FisRegD2H     Rfis;  // Register – Device to Host FIS
  Ne::Kernel::UInt8 Pad2[4];
  // 0x58
  FisDevBits Sdbfis;  // Set Device Bit FIS
  // 0x60
  Ne::Kernel::UInt8 Ufis[64];
  // 0xA0
  Ne::Kernel::UInt8 Rsv[0x100 - 0xA0];
} HbaFis;

typedef struct HbaPrdtEntry final {
  Ne::Kernel::UInt32 Dba;        // Data base address
  Ne::Kernel::UInt32 Dbau;       // Data base address upper 32 bits
  Ne::Kernel::UInt32 Reserved0;  // Reserved
  // DW3
  Ne::Kernel::UInt32 Dbc : 22;       // Byte count, 4M max
  Ne::Kernel::UInt32 Reserved1 : 9;  // Reserved
  Ne::Kernel::UInt32 Ie : 1;         // Interrupt on completion
} HbaPrdtEntry;

typedef struct HbaCmdTbl final {
  Ne::Kernel::UInt8       Cfis[64];  // Command FIS
  Ne::Kernel::UInt8       Acmd[16];  // ATAPI command, 12 or 16 bytes
  Ne::Kernel::UInt8       Rsv[48];   // Reserved
  struct HbaPrdtEntry Prdt[1];   // Physical region descriptor table entries, 0 ~ 65535
} HbaCmdTbl;

/// @brief Initializes an AHCI disk.
/// @param PortsImplemented the amount of port that have been detected.
/// @return
Ne::Kernel::Boolean drv_std_init(Ne::Kernel::UInt16& PortsImplemented);

Ne::Kernel::Boolean drv_std_detected(Ne::Kernel::Void);

/// @brief Read from AHCI disk.
/// @param lba
/// @param buf
/// @param sector_sz
/// @param buf_sz
/// @return
Ne::Kernel::Void drv_std_read(Ne::Kernel::UInt64 lba, Ne::Kernel::Char* buf, Ne::Kernel::SizeT sector_sz,
                          Ne::Kernel::SizeT buf_sz);

/// @brief Write to AHCI disk.
/// @param lba
/// @param buf
/// @param sector_sz
/// @param buf_sz
/// @return
Ne::Kernel::Void drv_std_write(Ne::Kernel::UInt64 lba, Ne::Kernel::Char* buf, Ne::Kernel::SizeT sector_sz,
                           Ne::Kernel::SizeT buf_sz);

/// @brief Gets the sector count from AHCI disk.
Ne::Kernel::SizeT drv_std_get_sector_count();

/// @brief Gets the AHCI disk size.
Ne::Kernel::SizeT drv_std_get_size();

/// @brief Checks if the drive has completed the command.
Ne::Kernel::Bool drv_is_ready(void);

/* EOF */

#endif
