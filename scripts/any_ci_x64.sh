#!/bin/sh

export AHCI_SUPPORT=1
export ATA_PIO_SUPPORT=
export ATA_DMA_SUPPORT=
export DEBUG_SUPPORT=1

cd private/minkernel
make -f amd64-desktop.make  all
