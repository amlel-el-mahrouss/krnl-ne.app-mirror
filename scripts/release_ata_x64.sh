#!/bin/sh

export ATA_PIO_SUPPORT=1

cd private/minkernel
make -f amd64-desktop.make  all
cd ../minloader
make -f amd64-desktop.make  all
make -f amd64-desktop.make disk
cd ../../
./tools/kimg.py ./private/minloader/private/nekernel-esp.img ./private/minloader/private/root
cd private/minloader
make -f amd64-desktop.make -j 8 run-efi-amd64-ata-pio