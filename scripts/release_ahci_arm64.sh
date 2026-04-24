#!/bin/sh

export AHCI_SUPPORT=1
export ATA_PIO_SUPPORT=
export ATA_DMA_SUPPORT=
export DEBUG_SUPPORT=

cd private/minkernel
make -f arm64-desktop.make  all
cd ../minloader
make -f arm64-desktop.make  all
make -f arm64-desktop.make disk
cd ../../
./tools/kimg.py ./private/minloader/private/nekernel-esp.img ./private/minloader/private/root
cd private/minloader
make -f arm64-desktop.make -j 8 run-efi-arm64
