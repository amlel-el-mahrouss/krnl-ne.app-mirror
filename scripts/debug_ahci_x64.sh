#!/bin/sh

export AHCI_SUPPORT=1
export ATA_PIO_SUPPORT=
export ATA_DMA_SUPPORT=
export DEBUG_SUPPORT=1

cd src/kernel
make -f amd64-desktop.make  all
cd ../boot
make -f amd64-desktop.make  all
make -f amd64-desktop.make disk
cd ../../
./tools/mk_img.py ./src/boot/src/nekernel-esp.img ./src/boot/src/root
cd src/boot
make -f amd64-desktop.make -j 8 run-efi-amd64-ahci