// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel
#ifndef KERNELKIT_INC_PE_H
#define KERNELKIT_INC_PE_H

#include <NeKit/Config.h>

#define kPeSignature (0x00004550)

#define kPeMagic32 (0x010b)
#define kPeMagic64 (0x020b)

#define kPeMachineAMD64 (0x8664)
#define kPeMachineARM64 (0xaa64)

#define kNeKernelPESubsystem (0x11)

typedef struct LDR_EXEC_HEADER final {
  Kernel::UInt32 Signature;
  Kernel::UInt16 Machine;
  Kernel::UInt16 NumberOfSections;
  Kernel::UInt32 TimeDateStamp;
  Kernel::UInt32 PointerToSymbolTable;
  Kernel::UInt32 NumberOfSymbols;
  Kernel::UInt16 SizeOfOptionalHeader;
  Kernel::UInt16 Characteristics;
} LDR_EXEC_HEADER, *LDR_EXEC_HEADER_PTR;

typedef struct LDR_OPTIONAL_HEADER final {
  Kernel::UInt16 Magic;  // 0x010b - PE32, 0x020b - PE32+ (64 bit)
  Kernel::UInt8  MajorLinkerVersion;
  Kernel::UInt8  MinorLinkerVersion;
  Kernel::UInt32 SizeOfCode;
  Kernel::UInt32 SizeOfInitializedData;
  Kernel::UInt32 SizeOfUninitializedData;
  Kernel::UInt32 AddressOfEntryPoint;
  Kernel::UInt32 BaseOfCode;
  Kernel::UInt64 ImageBase;
  Kernel::UInt32 SectionAlignment;
  Kernel::UInt32 FileAlignment;
  Kernel::UInt16 MajorOperatingSystemVersion;
  Kernel::UInt16 MinorOperatingSystemVersion;
  Kernel::UInt16 MajorImageVersion;
  Kernel::UInt16 MinorImageVersion;
  Kernel::UInt16 MajorSubsystemVersion;
  Kernel::UInt16 MinorSubsystemVersion;
  Kernel::UInt32 Win32VersionValue;
  Kernel::UInt32 SizeOfImage;
  Kernel::UInt32 SizeOfHeaders;
  Kernel::UInt32 CheckSum;
  Kernel::UInt16 Subsystem;
  Kernel::UInt16 DllCharacteristics;
  Kernel::UInt32 SizeOfStackReserve;
  Kernel::UInt32 SizeOfStackCommit;
  Kernel::UInt32 SizeOfHeapReserve;
  Kernel::UInt32 SizeOfHeapCommit;
  Kernel::UInt32 LoaderFlags;
  Kernel::UInt32 NumberOfRvaAndSizes;
} LDR_OPTIONAL_HEADER, *LDR_OPTIONAL_HEADER_PTR;

typedef struct LDR_SECTION_HEADER final {
  Kernel::Char   Name[8];
  Kernel::UInt32 VirtualSize;
  Kernel::UInt32 VirtualAddress;
  Kernel::UInt32 SizeOfRawData;
  Kernel::UInt32 PointerToRawData;
  Kernel::UInt32 PointerToRelocations;
  Kernel::UInt32 PointerToLineNumbers;
  Kernel::UInt16 NumberOfRelocations;
  Kernel::UInt16 NumberOfLinenumbers;
  Kernel::UInt32 Characteristics;
} LDR_SECTION_HEADER, *LDR_SECTION_HEADER_PTR;

enum kExecDataDirParams {
  kExecExport,
  kExecImport,
  kExecInvalid,
  kExecCount,
};

typedef struct LDR_EXPORT_DIRECTORY {
  Kernel::UInt32 Characteristics;
  Kernel::UInt32 TimeDateStamp;
  Kernel::UInt16 MajorVersion;
  Kernel::UInt16 MinorVersion;
  Kernel::UInt32 Name;
  Kernel::UInt32 Base;
  Kernel::UInt32 NumberOfFunctions;
  Kernel::UInt32 NumberOfNames;
  Kernel::UInt32 AddressOfFunctions;  // export table rva
  Kernel::UInt32 AddressOfNames;
  Kernel::UInt32 AddressOfNameOrdinal;  // ordinal table rva
} LDR_EXPORT_DIRECTORY, *LDR_EXPORT_DIRECTORY_PTR;

typedef struct LDR_IMPORT_DIRECTORY {
  union {
    Kernel::UInt32 Characteristics;
    Kernel::UInt32 OriginalFirstThunk;
  };
  Kernel::UInt32 TimeDateStamp;
  Kernel::UInt32 ForwarderChain;
  Kernel::UInt32 NameRva;
  Kernel::UInt32 ThunkTableRva;
} LDR_IMPORT_DIRECTORY, *LDR_IMPORT_DIRECTORY_PTR;

typedef struct LDR_DATA_DIRECTORY {
  Kernel::UInt32 VirtualAddress;
  Kernel::UInt32 Size;
} LDR_DATA_DIRECTORY, *LDR_DATA_DIRECTORY_PTR;

typedef struct LDR_IMAGE_HEADER {
  LDR_EXEC_HEADER     Header;
  LDR_OPTIONAL_HEADER OptHdr;
} LDR_IMAGE_HEADER, *LDR_IMAGE_HEADER_PTR;

enum {
  kUserSection  = 0x00000020,
  kPEResourceId = 0xFFaadd00,
};

#endif /* ifndef KERNELKIT_INC_PE_H */
