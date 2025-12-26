/* ========================================

  Copyright (C) 2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.

======================================== */

#ifndef MSGKIT_SERVER_H
#define MSGKIT_SERVER_H

#ifdef __cplusplus
#include <public/frameworks/CoreFoundation.fwrk/headers/String.h>
#else
#include <libSystem/SystemKit/System.h>
#endif

/// @author Amlal El Mahrouss
/// @file Server.h
/// @brief libMsg LISP system.

struct LIBMSG_EXPR;

/// @brief an expression chain of LibMSG.
struct LIBMSG_EXPR final {
#ifdef __cplusplus
  CF::CFString* l_key{nullptr};
  CF::CFString* l_value{nullptr};
#else
  // if we use C, we won't know about CF, so let's make those private.
  VoidPtr l_private_data[2]{nullptr};
#endif

  LIBMSG_EXPR* l_head{nullptr};
  LIBMSG_EXPR* l_tail{nullptr};
  LIBMSG_EXPR* l_child{nullptr};
};

/// @brief Function type for LibMSG lisp.
typedef Void (*libmsg_func_type)(struct LIBMSG_EXPR* self, VoidPtr arg, SizeT arg_size);

IMPORT_C Void   libmsg_init_library(libmsg_func_type* funcs, SizeT cnt);
IMPORT_C UInt32 libmsg_close_library(Void);
IMPORT_C UInt32 libmsg_eval_expr(struct LIBMSG_EXPR* head);

#endif
