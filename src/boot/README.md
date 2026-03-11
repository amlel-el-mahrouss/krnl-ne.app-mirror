# The BootZ Bootloader

A multi-platform bootloader written in C++ with modules support.

## Design Rationale:

BootZ like NeKernel is designed to be modular and production-ready.

BootZ reads a `kernel` image before trying to run it. If it fails to find one, it will summon a `bootnet` module.

Error Handling is very important as well, that's why we try before commiting (see BootKit.h)
