;; /*
;; *	========================================================
;; *
;; *	libSystem/private/SystemCallsABI+AMD64.asm
;; * 	Copyright (C) 2024-2025, Amlal El Mahrouss, licensed under the Apache 2.0 license.
;; *
;; * 	========================================================
;; */

[bits 64]

section .text

global nesys_syscall_arg_1
global nesys_syscall_arg_2
global nesys_syscall_arg_3
global nesys_syscall_arg_4

nesys_syscall_arg_1:
    push rbp
    mov rbp, rsp

    mov r8, rcx

    xor rax, rax

    syscall

    pop rbp

    ret

nesys_syscall_arg_2:
    push rbp
    mov rbp, rsp

    mov r8, rcx
    mov r9, rdx

    xor rax, rax

    syscall

    pop rbp

    ret

nesys_syscall_arg_3:
    push rbp
    mov rbp, rsp

    mov r8, rcx
    mov r9, rdx
    mov r10, rbx

    xor rax, rax

    syscall

    pop rbp

    ret

nesys_syscall_arg_4:
    push rbp
    mov rbp, rsp

    mov rax, r8

    mov r8, rcx
    mov r9, rdx
    mov r10, rbx
    mov r11, rax

    xor rax, rax

    syscall

    pop rbp

    ret
