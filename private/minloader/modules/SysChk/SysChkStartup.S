;; /*
;; *	========================================================
;; *
;; *	BootZ
;; * 	Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.
;; *
;; * 	========================================================
;; */

#ifdef __NE_AMD64__
.code64
.intel_syntax noprefix
#endif

#ifdef __NE_AMD64__

.section .text

.global ___chkstk_ms

___chkstk_ms:
    push rcx
    push rax

    cmp rax, 0x1000
    lea rcx, [rsp + 24]
    jb .last

.probe:
    sub rcx, 0x1000
    or rcx, 0

    sub rax, 0x1000
    cmp rax, 0x1000
    ja .probe

.last:
    sub rcx, rax
    or rcx, 0

    pop rax
    pop rcx

    ret
#endif

#define kTypeDriver 101
#define kArchAmd64 122
#define kHandoverMagic 0xBADAA

.section .ldr

.quad kHandoverMagic
.word kTypeDriver
.word 0
.word kArchAmd64
