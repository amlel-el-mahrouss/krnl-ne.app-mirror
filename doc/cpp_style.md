# NeKernel C++ Style Guide:

## Abstract:

NeKernel uses the Google C++ Style. But applied to low-level systems. We use C++20 and later throughout all of our stack. As we keep iterating over time we will amend this document.

## No Exceptions, Never.

NeKernel bans the usage of exceptions in ne_kernel, ne_bootz, etc.
Unless it is a program space and not to be deemed performance dependent.

## Privilege Templates and ZOA over C with Classes.

NeKernel doesn't limit itself to a C++ paradigm, as we may find fitting solutions by exploring other patterns.

