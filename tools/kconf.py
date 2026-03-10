#! /usr/bin/env python3
# -*- coding: utf-8 -*-

import os, json, sys

if __name__ == '__main__':
    print("kconf: running kernel as standalone...")
    os.system("../scripts/debug_ahci_x64.sh")
    sys.exit(0)




