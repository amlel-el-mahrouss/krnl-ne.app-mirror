#!/bin/sh

# LOG HISTORY:
# 03/25/25: Add 'disk' build step.
# 04/05/25: Improve and fix script.

cd private/bootz/modules/SysChk
nebuild amd64-ahci-epm.json
cd ../
cd BootNet
nebuild amd64.json
cd ../
cd MemoryTest
nebuild amd64.json
