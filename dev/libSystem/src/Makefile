##################################################
# (c) Amlal El Mahrouss and NeKernel contributors, licensed under the Apache 2.0 license.
# This file is for libSystem.dll's syscall stubs.
##################################################

ASM=nasm
FLAGS=-f win64

.PHONY: error
error:
	@echo "==> Invalid recipe."
	@echo "==> Use libsys_asm_io_<arch> instead."

.PHONY: libsys_asm_io_x64
libsys_asm_io_x64:
	$(ASM) $(FLAGS) SystemCallsABI+AMD64.asm -o SystemCallsABI+AMD64.stub.obj
