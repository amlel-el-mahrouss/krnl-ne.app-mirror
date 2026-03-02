// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <KernelKit/Timer.h>

/// ================================================================================
/// @note The ITimer class is meant to be generic.
///! @author Amlal El Mahrouss (amlal@nekernel.org)
/// ================================================================================

using namespace Kernel;

SoftwareTimer::SoftwareTimer(Int64 seconds) : fWaitFor(seconds) {
  fDigitalTimer = new UIntPtr();
  MUST_PASS(fDigitalTimer);
}

SoftwareTimer::~SoftwareTimer() {
  delete fDigitalTimer;
  fDigitalTimer = nullptr;

  fWaitFor = 0;
}

BOOL SoftwareTimer::Wait() {
  if (fWaitFor < 1L) return NO;

  while (*fDigitalTimer < (*fDigitalTimer + fWaitFor)) {
    ++(*fDigitalTimer);
  }

  return YES;
}
