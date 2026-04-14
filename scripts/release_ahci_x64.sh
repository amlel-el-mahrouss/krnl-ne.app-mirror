#!/bin/sh

export AHCI_SUPPORT=1

cd private/minkernel
make -f amd64-desktop.make  all
cd ../bootz
make -f amd64-desktop.make  all
make -f amd64-desktop.make disk
cd ../../
./tools/kimg.py ./private/bootz/private/nekernel-esp.img ./private/bootz/private/root
cd src/bootz
make -f amd64-desktop.make -j 8 run-efi-amd64-ahci