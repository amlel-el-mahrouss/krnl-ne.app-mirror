;; // SPDX-License-Identifier: Apache-2.0
;; // Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
;; // Licensed under the Apache License, Version 2.0 (see LICENSE file)
;; // Official repository: https://github.com/ne-app-eu/krnl

[bits 64]

[global rt_install_tib]

section .text

;; changed: rs, fs
;; expected: rcx, rdx

rt_install_tib:
	mov rcx, gs ;; TIB -> Thread Information Block
	mov rdx, fs ;; PIB -> Process Information Block
	ret

;; //////////////////////////////////////////////////// ;;
