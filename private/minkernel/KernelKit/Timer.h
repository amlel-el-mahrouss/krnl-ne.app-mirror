// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#ifndef KERNELKIT_TIMER_H
#define KERNELKIT_TIMER_H

#include <ArchKit/ArchKit.h>
#include <KernelKit/KPC.h>

namespace Kernel {

class SoftwareTimer;
class HardwareTimer;
class ITimer;

inline constexpr Int16 kTimeUnit = 1000;

class ITimer {
 public:
  /// @brief Default constructor
  explicit ITimer() = default;
  virtual ~ITimer() = default;

 public:
  NE_COPY_DEFAULT(ITimer)

 public:
  virtual BOOL Wait();
};

class SoftwareTimer final : public ITimer {
 public:
  explicit SoftwareTimer(Int64 seconds);
  ~SoftwareTimer() override;

 public:
  NE_COPY_DEFAULT(SoftwareTimer)

 public:
  BOOL Wait() override;

 private:
  UIntPtr* fDigitalTimer{nullptr};
  Int64    fWaitFor{0L};
};

class HardwareTimer final : public ITimer {
 public:
  explicit HardwareTimer(UInt64 seconds);
  ~HardwareTimer() override;

 public:
  NE_COPY_DEFAULT(HardwareTimer)

 public:
  BOOL Wait() override;

 private:
  volatile UInt8* fDigitalTimer{nullptr};
  Int64           fWaitFor{0};
};

inline constexpr UInt64 rtl_microseconds(UInt64 time) {
  if (time < 1) return 0;
  return time / kTimeUnit;
}

inline constexpr UInt64 rtl_milliseconds(UInt64 time) {
  if (time < 1) return 0;
  return time;
}

}  // namespace Kernel

#endif  // !KERNELKIT_TIMER_H
