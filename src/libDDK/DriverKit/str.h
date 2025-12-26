/* ========================================

  Copyright Amlal El Mahrouss.

  Purpose: DDK Strings.

======================================== */

#ifndef DRIVERKIT_STR_H
#define DRIVERKIT_STR_H

#include <DriverKit/ddk.h>

/// @brief DDK equivalent of POSIX's string.h
/// @file str.h

DDK_EXTERN size_t kstrlen(const char* in);
DDK_EXTERN int    kstrncpy(char* dst, const char* src, size_t len);

#endif
