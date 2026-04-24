;; /*
;; *	========================================================
;; *
;; *	MinChk
;; * 	Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.
;; *
;; * 	========================================================
;; */

#ifdef __NE_AMD64__
.code64
.intel_syntax noprefix
#endif

#define kTypeDriver 101
#define kArchAmd64 122
#define kHandoverMagic 0xBADCC

.section .ldr

.quad kHandoverMagic
.word kTypeDriver
.word 0
.word kArchAmd64