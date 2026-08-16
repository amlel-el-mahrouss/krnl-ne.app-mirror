#! /bin/sh

rm private/minloader/src/NE_BOOT.vdi
VBoxManage convertfromraw private/minloader/src/nekernel-esp.img private/minloader/src/NE_BOOT.vdi --format VDI

