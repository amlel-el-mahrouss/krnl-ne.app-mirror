// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef KERNELKIT_DEVICEMGR_H
#define KERNELKIT_DEVICEMGR_H

/* @note Device Mgr. */
/* @file KernelKit/DeviceMgr.h */
/* @brief Device abstraction and I/O buffer. */

#include <NeKit/ErrorOr.h>
#include <NeKit/Ref.h>

#define kDeviceMgrRootDirPath "/devices/"

#define NE_DEVICE : public ::Kernel::IDevice

// Last Rev: Wed, May  27, 2025  6:22 PM

namespace Kernel {

template <typename T>
class IDevice;

template <typename T>
class IOBuf;

/***********************************************************************************/
/// @brief Device contract interface, represents an HW device.
/***********************************************************************************/
template <typename T>
class IDevice {
 public:
  IDevice() = default;

  IDevice(void (*Out)(IDevice<T>*, T), void (*In)(IDevice<T>*, T)) : fOut(Out), fIn(In) {}
  virtual ~IDevice() = default;

 public:
  using Type      = T;
  using TypeRef   = T&;
  using ConstType = const T&;
  using TypePtr   = T*;

  NE_COPY_DEFAULT(IDevice)

 public:
  virtual IDevice<T>& operator<<(T Data) {
    fOut(this, Data);
    return *this;
  }

  virtual IDevice<T>& operator>>(T Data) {
    fIn(this, Data);
    return *this;
  }

  virtual const char* Name() const { return kDeviceMgrRootDirPath "null"; }

  operator bool() { return fOut && fIn; }

  Bool operator!() { return !fOut || !fIn; }

 protected:
  Void (*fOut)(IDevice<T>*, T Data) = {nullptr};
  Void (*fIn)(IDevice<T>*, T Data)  = {nullptr};
};

///
/// @brief Input Output abstract class.
/// Used mainly to communicate between OS to hardware.
///
template <typename T>
class IOBuf final {
 public:
  explicit IOBuf(T dma_addr) : fData(dma_addr) {
    // At least pass something valid when instancating this struct.
    MUST_PASS(fData);
  }

  IOBuf& operator=(const IOBuf<T>&) = default;
  IOBuf(const IOBuf<T>&)            = default;

  ~IOBuf() = default;

 public:
  template <typename R>
  R operator->() const {
    return fData;
  }

  template <typename R>
  R& operator[](Size index) const {
    return fData[index];
  }

 private:
  T fData;
};

///! @brief Device enum types.
enum {
  kDeviceTypeInvalid = 0,
  kDeviceTypeIDE     = 100,
  kDeviceTypeEthernet,
  kDeviceTypeWiFi,
  kDeviceTypeFW,
  kDeviceTypeBT,
  kDeviceTypeRS232,
  kDeviceTypeSCSI,
  kDeviceTypeAHCI,
  kDeviceTypeMBCI,
  kDeviceTypeATA,
  kDeviceTypeUSB,
  kDeviceTypeAPM,  // Adv. Pwr. Mgmt.
  kDeviceTypePCI,
  kDeviceTypeVGA,
  kDeviceTypeGPU,
  kDeviceTypeRandom,
  kDeviceTypeIPC,
  kDeviceTypeCount = kDeviceTypeIPC - kDeviceTypeIDE + 1,
};

}  // namespace Kernel

#endif
