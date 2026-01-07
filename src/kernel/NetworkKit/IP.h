// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef NETWORKKIT_IP_H
#define NETWORKKIT_IP_H

#include <KernelKit/DebugOutput.h>
#include <NeKit/Config.h>
#include <NeKit/KString.h>
#include <NeKit/Ref.h>

namespace Kernel {
class RawIPAddress6;
class RawIPAddress;
class IPFactory;

class RawIPAddress final {
 private:
  explicit RawIPAddress(UInt8 bytes[4]);

 public:
  ~RawIPAddress() = default;

  RawIPAddress& operator=(const RawIPAddress&) = default;
  RawIPAddress(const RawIPAddress&)            = default;

  UInt8* Address();

  UInt8& operator[](const Size& index);

  BOOL operator==(const RawIPAddress& ipv6);
  BOOL operator!=(const RawIPAddress& ipv6);

 private:
  UInt8 fAddr[4] = {};

  friend IPFactory;  // it is the one creating these addresses, thus this
                     // is why the constructors are private.
};

/**
 * @brief IPv6 address.
 */
class RawIPAddress6 final {
 private:
  explicit RawIPAddress6(UInt8 Bytes[16]);

 public:
  ~RawIPAddress6() = default;

  RawIPAddress6& operator=(const RawIPAddress6&) = default;
  RawIPAddress6(const RawIPAddress6&)            = default;

  UInt8* Address();

  UInt8& operator[](const Size& index);

  bool operator==(const RawIPAddress6& ipv6);
  bool operator!=(const RawIPAddress6& ipv6);

 private:
  UInt8 fAddr[16] = {};

  friend IPFactory;
};

/**
 * @brief IP Creation helpers
 */
class IPFactory final {
 public:
  static ErrorOr<KBasicString<UInt8>> ToKString(Ref<RawIPAddress6>& ipv6);
  static ErrorOr<KBasicString<UInt8>> ToKString(Ref<RawIPAddress>& ipv4);
  static bool                         IpCheckVersion4(const Char* ip);
};
}  // namespace Kernel

#endif
