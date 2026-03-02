// Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/ne-foss-org/nekernel

#include <libSystem/SystemKit/Err.h>
#include <libSystem/SystemKit/System.h>

SInt32 _NeMain(SInt32 argc, Char* argv[]) {
  LIBSYS_UNUSED(argc);
  LIBSYS_UNUSED(argv);

  if (argc != 3) {
    PrintOut(nullptr, "HELP: manual <tutorial_name> <output_name>\n");
    return EXIT_FAILURE;
  }

  IORef fileHtml = IoOpenFile(argv[2], nullptr);

  if (fileHtml) {
    IORef printRef = PrintCreate();

    PrintOut(printRef, "<HTML>");
    PrintOut(printRef, "<PRE>");

    IORef in = PrintGet(argv[1]);

    MUST_PASS(in);

    Char buf[1024] = {0};

    while (PrintIn(printRef, "%s:1024", buf) == kErrorSuccess) PrintOut(printRef, "%s", buf);

    PrintRelease(in);

    PrintOut(printRef, "</PRE>");
    PrintOut(printRef, "</HTML>");

    IoWriteFile(fileHtml, printRef, PrintSize(printRef));
    PrintRelease(printRef);

    IoCloseFile(fileHtml);
  }

  return EXIT_SUCCESS;
}