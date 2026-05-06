;; // SPDX-License-Identifier: Apache-2.0
;; // Copyright 2024-2026, Amlal El Mahrouss (amlal@nekernel.org)
;; // Licensed under the Apache License, Version 2.0 (see LICENSE file)
;; // Official repository: https://github.com/ne-foss/nekernel

[bits 64]

%define kTypeKernel 100
%define kArchAmd64 122
%define kHandoverMagic 0xBADAA

global _HandoverMagic
global _HandoverType
global _HandoverPad
global _HandoverArch

section .botz

_HandoverMagic:
    dq kHandoverMagic
_HandoverType:
    dw kTypeKernel
_HandoverPad:
    dw 0
_HandoverArch:
    dw kArchAmd64
