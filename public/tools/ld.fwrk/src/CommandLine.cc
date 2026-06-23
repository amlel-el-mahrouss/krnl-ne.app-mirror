/*
 * Created on Thu Oct 17 08:00:42 CEST 2024
 *
 * Copyright (c) 2024-2025 Amlal El Mahrouss
 */

#include <SystemKit/System.h>

/// @brief This program loads a code framework into Ne::Kernel's memory.

SInt32 _NeMain(SInt32 argc, Char* argv[]) {
  LIBSYS_UNUSED(argc);
  LIBSYS_UNUSED(argv);

  PrintOut(nullptr, "%s", "ld.fwrk: Framework Loader.\n");
  PrintOut(nullptr, "%s",
           "ld.fwrk: © 2024-2025 Amlal El Mahrouss, Licensed under the Apache 2.0 license.\n");

  return EXIT_FAILURE;
}
