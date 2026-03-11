#!/bin/sh

# LOG HISTORY:
# 03/25/25: Add 'disk' build step.
# 04/05/25: Improve and fix script.

cd src/boot/modules/SysChk
nebuild arm64-ahci-epm.json
cd ../
cd BootNet
nebuild arm64.json
