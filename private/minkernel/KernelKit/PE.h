// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef KERNELKIT_INC_PE_H
#define KERNELKIT_INC_PE_H

#include <NeKit/Config.h>

#define kPeSignature (0x00004550)

#define kPeMagic32 (0x010b)
#define kPeMagic64 (0x020b)

#define kPeMachineAMD64 (0x8664)
#define kPeMachineARM64 (0xaa64)

#define kNeKernelPESubsystem (0x11)

/* Sanity cap for untrusted images. */
#define kPeMaxImageSz (0x4000000U)

typedef struct LDR_EXEC_HEADER final {
  Ne::Kernel::UInt32 Signature;
  Ne::Kernel::UInt16 Machine;
  Ne::Kernel::UInt16 NumberOfSections;
  Ne::Kernel::UInt32 TimeDateStamp;
  Ne::Kernel::UInt32 PointerToSymbolTable;
  Ne::Kernel::UInt32 NumberOfSymbols;
  Ne::Kernel::UInt16 SizeOfOptionalHeader;
  Ne::Kernel::UInt16 Characteristics;
} LDR_EXEC_HEADER, *LDR_EXEC_HEADER_PTR;

typedef struct LDR_OPTIONAL_HEADER final {
  Ne::Kernel::UInt16 Magic;  // 0x010b - PE32, 0x020b - PE32+ (64 bit)
  Ne::Kernel::UInt8  MajorLinkerVersion;
  Ne::Kernel::UInt8  MinorLinkerVersion;
  Ne::Kernel::UInt32 SizeOfCode;
  Ne::Kernel::UInt32 SizeOfInitializedData;
  Ne::Kernel::UInt32 SizeOfUninitializedData;
  Ne::Kernel::UInt32 AddressOfEntryPoint;
  Ne::Kernel::UInt32 BaseOfCode;
  Ne::Kernel::UInt64 ImageBase;
  Ne::Kernel::UInt32 SectionAlignment;
  Ne::Kernel::UInt32 FileAlignment;
  Ne::Kernel::UInt16 MajorOperatingSystemVersion;
  Ne::Kernel::UInt16 MinorOperatingSystemVersion;
  Ne::Kernel::UInt16 MajorImageVersion;
  Ne::Kernel::UInt16 MinorImageVersion;
  Ne::Kernel::UInt16 MajorSubsystemVersion;
  Ne::Kernel::UInt16 MinorSubsystemVersion;
  Ne::Kernel::UInt32 Win32VersionValue;
  Ne::Kernel::UInt32 SizeOfImage;
  Ne::Kernel::UInt32 SizeOfHeaders;
  Ne::Kernel::UInt32 CheckSum;
  Ne::Kernel::UInt16 Subsystem;
  Ne::Kernel::UInt16 DllCharacteristics;
  Ne::Kernel::UInt32 SizeOfStackReserve;
  Ne::Kernel::UInt32 SizeOfStackCommit;
  Ne::Kernel::UInt32 SizeOfHeapReserve;
  Ne::Kernel::UInt32 SizeOfHeapCommit;
  Ne::Kernel::UInt32 LoaderFlags;
  Ne::Kernel::UInt32 NumberOfRvaAndSizes;
} LDR_OPTIONAL_HEADER, *LDR_OPTIONAL_HEADER_PTR;

typedef struct LDR_SECTION_HEADER final {
  Ne::Kernel::Char   Name[8];
  Ne::Kernel::UInt32 VirtualSize;
  Ne::Kernel::UInt32 VirtualAddress;
  Ne::Kernel::UInt32 SizeOfRawData;
  Ne::Kernel::UInt32 PointerToRawData;
  Ne::Kernel::UInt32 PointerToRelocations;
  Ne::Kernel::UInt32 PointerToLineNumbers;
  Ne::Kernel::UInt16 NumberOfRelocations;
  Ne::Kernel::UInt16 NumberOfLinenumbers;
  Ne::Kernel::UInt32 Characteristics;
} LDR_SECTION_HEADER, *LDR_SECTION_HEADER_PTR;

enum kExecDataDirParams {
  kExecExport,
  kExecImport,
  kExecInvalid,
  kExecCount,
};

typedef struct LDR_EXPORT_DIRECTORY {
  Ne::Kernel::UInt32 Characteristics;
  Ne::Kernel::UInt32 TimeDateStamp;
  Ne::Kernel::UInt16 MajorVersion;
  Ne::Kernel::UInt16 MinorVersion;
  Ne::Kernel::UInt32 Name;
  Ne::Kernel::UInt32 Base;
  Ne::Kernel::UInt32 NumberOfFunctions;
  Ne::Kernel::UInt32 NumberOfNames;
  Ne::Kernel::UInt32 AddressOfFunctions;  // export table rva
  Ne::Kernel::UInt32 AddressOfNames;
  Ne::Kernel::UInt32 AddressOfNameOrdinal;  // ordinal table rva
} LDR_EXPORT_DIRECTORY, *LDR_EXPORT_DIRECTORY_PTR;

typedef struct LDR_IMPORT_DIRECTORY {
  union {
    Ne::Kernel::UInt32 Characteristics;
    Ne::Kernel::UInt32 OriginalFirstThunk;
  };
  Ne::Kernel::UInt32 TimeDateStamp;
  Ne::Kernel::UInt32 ForwarderChain;
  Ne::Kernel::UInt32 NameRva;
  Ne::Kernel::UInt32 ThunkTableRva;
} LDR_IMPORT_DIRECTORY, *LDR_IMPORT_DIRECTORY_PTR;

typedef struct LDR_DATA_DIRECTORY {
  Ne::Kernel::UInt32 VirtualAddress;
  Ne::Kernel::UInt32 Size;
} LDR_DATA_DIRECTORY, *LDR_DATA_DIRECTORY_PTR;

typedef struct LDR_IMAGE_HEADER {
  LDR_EXEC_HEADER     Header;
  LDR_OPTIONAL_HEADER OptHdr;
} LDR_IMAGE_HEADER, *LDR_IMAGE_HEADER_PTR;

enum : long long {
  kUserSection  = 0x00000020,
  kPEResourceId = 0xFFaadd00,
};

#endif /* ifndef KERNELKIT_INC_PE_H */
