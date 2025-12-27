// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef __KERNEL_KIT_MSDOS_H__
#define __KERNEL_KIT_MSDOS_H__

#include <KernelKit/PE.h>
#include <NeKit/Config.h>

// Last Rev
// Sat Feb 24 CET 2024

#define kMagMz0 'M'
#define kMagMz1 'Z'

typedef Kernel::UInt32 DosWord;
typedef Kernel::Long   DosLong;

typedef struct _DosHeader {
  Kernel::UInt8 eMagic[2];
  DosWord       eMagLen;
  DosWord       ePagesCount;
  DosWord       eCrlc;
  DosWord       eCParHdr;
  DosWord       eMinAlloc;
  DosWord       eMaxAlloc;
  DosWord       eStackSeg;
  DosWord       eStackPtr;
  DosWord       eChksum;
  DosWord       eIp;
  DosWord       eCs;
  DosWord       eLfarlc;
  DosWord       eOvno;
  DosWord       eRes[4];
  DosWord       eOemid;
  DosWord       eOeminfo;
  DosWord       eRes2[10];
  DosLong       eLfanew;
} DosHeader, *DosHeaderPtr;

#endif /* ifndef __KERNEL_KIT_MSDOS_H__ */
