#!/bin/sh

export AHCI_SUPPORT=1
export ATA_PIO_SUPPORT=
export ATA_DMA_SUPPORT=
export DEBUG_SUPPORT=

cd private/minkernel
make -f arm64-desktop.make  all
cd ../bootz
make -f arm64-desktop.make  all
make -f arm64-desktop.make disk
cd ../../
./tools/kimg.py ./private/bootz/private/nekernel-esp.img ./private/bootz/private/root
cd src/bootz
make -f arm64-desktop.make -j 8 run-efi-arm64
