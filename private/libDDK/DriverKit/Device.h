// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-app-eu/krnl

#ifndef DRIVERKIT_DEV_H
#define DRIVERKIT_DEV_H

#include <DriverKit/DriverKit.h>

struct _DDK_DEVICE;

#define DDK_DEVICE_NAME_LEN (255)

#define DDK_TYPE_SOCKET (1)
#define DDK_TYPE_FILE (2)
#define DDK_TYPE_DEVICE (3)

#define DDK_SUB_TYPE_TCP (1) /* TCP/IP */
#define DDK_SUB_TYPE_UDP (2) /* Datagram  */
#define DDK_SUB_TYPE_BT (3)  /* Bluetooth */

/// @brief This enum takes care of the network stack.
enum {
  DDK_NET_DO_NOT_FLUSH,
  DDK_NET_FLUSH_NOW,
  DDK_NET_AUTO_FLUSH,
};

/// @brief Kernel Device driver.
typedef struct _DDK_DEVICE DDK_FINAL {
  char d_name[DDK_DEVICE_NAME_LEN];  // the device name. Could be /./DEVICE_NAME/
  int  d_type;
  int  d_subtype;
  void* (*d_read)(void* arg, int len);  // read from device.
  void (*d_write)(void* arg, int len);
  void (*d_wait)(void);                             // write to device.
  struct _DDK_DEVICE* (*d_open)(const char* path);  // open device.
  void (*d_close)(struct _DDK_DEVICE* dev);         // close device.
  void (*d_seek)(struct _DDK_DEVICE* dev, size_t off);
  size_t (*d_tell)(struct _DDK_DEVICE* dev);
} DDK_DEVICE, *DDK_DEVICE_PTR;

#define kopen kopen_dev
#define kclose kclose_dev

/// @brief Open a new device from path.
/// @param path the device's path.
DDK_EXTERN DDK_DEVICE_PTR kopen_dev(const char* path);

/// @brief Close any device.
/// @param device valid device.
DDK_EXTERN BOOL kclose_dev(DDK_DEVICE_PTR device);

#endif
