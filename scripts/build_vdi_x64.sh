#! /bin/sh

UUID_VDI=$(uuidgen)
echo $UUID_VDI

VBoxManage convertfromraw private/minloader/src/nekernel-esp.img private/minloader/src/$UUID_VDI.vdi --format VDI

echo $PWD/private/minloader/src/$UUID_VDI.vdi