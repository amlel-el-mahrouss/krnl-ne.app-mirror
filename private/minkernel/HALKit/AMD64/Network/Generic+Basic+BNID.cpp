// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#include <DmaKit/DmaPool.h>
#include <HALKit/AMD64/Processor.h>
#include <KernelKit/PCI/Iterator.h>
#include <modules/ACPI/ACPIFactoryInterface.h>

/// @note BNID (RTL8139' Basic Network Interface) driver

#define kNetDevID (0x8139)
#define kNetSubClass (0x10EC)

using namespace Kernel;
using namespace Kernel::HAL;

STATIC UInt16 kRTLIOBase = 0xFFFF;

STATIC UInt32                 kRXOffset     = 0UL;
STATIC constexpr CONST UInt32 kRXBufferSize = 8192 + 16 + 1500;

STATIC UInt8* kRXUpperLayer = nullptr;
STATIC UInt8* kRXBuffer     = nullptr;

STATIC PCI::Device kNetDev;

/***********************************************************************************/
///@brief BNID Init routine.
/***********************************************************************************/

EXTERN_C BOOL rtl_init_nic_rtl8139() {
  STATIC BOOL kTXRXEnabled = NO;
  if (kTXRXEnabled) return NO;

  PCI::Iterator iterator(Types::PciDeviceKind::NetworkController, 0x00);

  for (SizeT device_index = 0; device_index < NE_BUS_COUNT; ++device_index) {
    kNetDev = iterator[device_index].Leak();  // Leak device.

    if (kNetDev.VendorId() == kNetSubClass && kNetDev.DeviceId() == kNetDevID) {
      kNetDev.EnableMmio();
      kNetDev.BecomeBusMaster();

      break;
    }
  }

  kRTLIOBase = kNetDev.Bar(0);

  MUST_PASS(kRTLIOBase != 0xFFFF);

  kRXBuffer = reinterpret_cast<UInt8*>(rtl_dma_alloc(sizeof(UInt8) * kRXBufferSize, 0));

  /// Reset first.

  rt_out8(kRTLIOBase + 0x37, 0x10);

  UInt16 timeout = 0U;

  while (rt_in8(kRTLIOBase + 0x37) & 0x10) {
    ++timeout;
    if (timeout > 0x1000) break;
  }

  rt_out32(kRTLIOBase + 0x30, (UInt32) (UIntPtr) kRXBuffer);

  rt_out8(kRTLIOBase + 0x37, 0x0C);

  rt_out32(kRTLIOBase + 0x44, 0xF | (1 << 7));

  rt_out16(kRTLIOBase + 0x3C, 0x0005);

  kTXRXEnabled = YES;

  kout << "The Basic Network Interface Driver (BNID) has been initialized.\r";

  return YES;
}

/***********************************************************************************/
/// @brief BNID I/O interrupt handler.
/// @param rsp stack pointer.
/// @note This function is called when the device interrupts to retrieve network data.
/***********************************************************************************/

EXTERN_C Void rtl_rtl8139_interrupt_handler(UIntPtr rsp) {
  if (kRTLIOBase == 0xFFFF || kRTLIOBase == 0) return;

  NE_UNUSED(rsp);

  UInt16 status = rt_in16(kRTLIOBase + 0x3E);
  rt_out16(kRTLIOBase + 0x3E, status);

  if (status & 0x01) {
    // While we receive data.
    while ((rt_in8(kRTLIOBase + 0x37) & 0x01) == 0) {
      // We grab an offset from the RX buffer.
      UInt32 offset = kRXOffset % kRXBufferSize;

      // If the offset is too high, we reset it.
      if (offset >= (kRXBufferSize - 16)) {
        kRXOffset = 0UL;
        offset    = 0UL;
      }

      volatile UInt8* packet = kRXBuffer + offset + 4;
      UInt16          len    = *(UInt16*) (kRXBuffer + offset + 2);

      kRXUpperLayer[(offset + 4)] = *packet;
      kRXOffset += (len + 4);

      rt_out16(kRTLIOBase + 0x38, (UInt16) (kRXOffset - 16));
    }
  }

  if (!(status & 0x04)) {
    err_global_get() = kErrorNoNetwork;
  }
}

/***********************************************************************************/
/// @brief BNID get upper layer function
/// @return the upper layer.
/// @retval nullptr if no upper layer is set.
/// @retval pointer to the upper layer if set.
/***********************************************************************************/

EXTERN_C UInt8* rtl_rtl8139_get_upper_layer() {
  return kRXUpperLayer;
}

/***********************************************************************************/
/// @brief BNID set upper layer function
/// @param layer the upper layer.
/***********************************************************************************/

EXTERN_C BOOL rtl_rtl8139_set_upper_layer(UInt8* layer) {
  if (!layer) return NO;
  kRXUpperLayer = layer;

  return YES;
}
