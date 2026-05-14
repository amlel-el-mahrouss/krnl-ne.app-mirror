// SPDX-License-Identifier: Apache-2.0
// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss/nekernel

#include <SystemKit/Jail.h>
#include <SystemKit/System.h>

/// @author Amlal El Mahrouss
/// @brief OpenEnclave management tool

static JAIL* kJailSrv = nullptr;

SInt32 _NeMain(SInt32 argc, Char* argv[]) {
  LIBSYS_UNUSED(argc);
  LIBSYS_UNUSED(argv);

  kJailSrv = ::JailGetCurrent();

  MUST_PASS(kJailSrv);

  ::PrintOut(nullptr, "%s", "mgmt.oe: OpenEnclave Management Tool.");

  /// @note JailGetCurrent returns client as nullptr if we're not that client (we'll not be able to
  /// access the jail then)
  if (kJailSrv->fClient == nullptr) return EXIT_FAILURE;

  ::PrintOut(nullptr, "Jail-Hash: %ul", kJailSrv->fJailHash);
  ::PrintOut(nullptr, "Parent-ID: %ul", kJailSrv->fParentID);

  return EXIT_FAILURE;
}