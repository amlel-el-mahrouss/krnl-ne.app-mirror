// Copyright 2024-2025, Amlal El Mahrouss (amlal@nekernel.org)
// Licensed under the Apache License, Version 2.0 (see LICENSE file)
// Official repository: https://github.com/nekernel-org/nekernel

#ifndef CFKIT_UTILS_H
#define CFKIT_UTILS_H

#include <KernelKit/MSDOS.h>
#include <KernelKit/PE.h>

/// @brief CFKit namespace.
namespace Kernel::CF {
/// @brief Finds the PE header inside the blob.
inline auto ldr_find_exec_header(DosHeaderPtr ptrDos) -> LDR_EXEC_HEADER_PTR {
  if (!ptrDos) return nullptr;

  if (ptrDos->eMagic[0] != kMagMz0) return nullptr;

  if (ptrDos->eMagic[1] != kMagMz1) return nullptr;

#if defined(__NE_AMD64__)
  return (LDR_EXEC_HEADER_PTR) (VoidPtr) (&ptrDos->eLfanew + 1);
#else
  return (LDR_EXEC_HEADER_PTR) (VoidPtr) (&ptrDos->eLfanew);
#endif
}

/// @brief Finds the PE optional header inside the blob.
inline auto ldr_find_opt_exec_header(DosHeaderPtr ptrDos) -> LDR_OPTIONAL_HEADER_PTR {
  if (!ptrDos) return nullptr;

  auto exec = ldr_find_exec_header(ptrDos);

  if (!exec) return nullptr;

  return (LDR_OPTIONAL_HEADER_PTR) (VoidPtr) (&exec->Characteristics + 1);
}

/// @brief Finds the PE header inside the blob.
/// @note overloaded function.
inline auto ldr_find_exec_header(const Char* ptrDos) -> LDR_EXEC_HEADER_PTR {
  return ldr_find_exec_header((DosHeaderPtr) ptrDos);
}

/// @brief Finds the PE header inside the blob.
/// @note overloaded function.
inline auto ldr_find_opt_exec_header(const Char* ptrDos) -> LDR_OPTIONAL_HEADER_PTR {
  return ldr_find_opt_exec_header((DosHeaderPtr) ptrDos);
}
}  // namespace Kernel::CF

#endif  // ifndef CFKIT_UTILS_H
