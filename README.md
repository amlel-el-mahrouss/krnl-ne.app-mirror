<!-- Read Me of NeKernel -->

<div align="center">
  <img src="meta/media/nekernel.png" alt="NeKernel Logo" width="180"/>
  <p>
    <b>Modern, Modular, and Secure Microkernel for Next-Generation Systems</b>
  </p>
  <p>
    <a href="https://github.com/nekernel-org/nekernel/actions/workflows/boot-pio-dev.yml"><img src="https://github.com/nekernel-org/nekernel/actions/workflows/boot-pio.yml/badge.svg" alt="CI"></a>
    <a href="https://github.com/nekernel-org/nekernel/actions/workflows/kernel-ahci-dev.yml"><img src="https://github.com/nekernel-org/nekernel/actions/workflows/kernel-ahci.yml/badge.svg" alt="CI"></a>
    <a href="LICENSE"><img src="https://img.shields.io/badge/License-Apache--2.0-blue.svg" alt="License"></a>
  </p>
</div>

<img src="meta/media/demo.gif" alt="NeKernel Logo" width="1280"/>

---

## Overview

**NeKernel** is a modern, multi-platform microkernel designed for security, modularity, and performance. It features a custom VFS, advanced memory management, a flexible DDK (Driver Development Kit), and robust userland tools. NeKernel is built for research, education, and next-generation OS development.

---

## Getting Started

### **Requirements**
- [MinGW](https://www.mingw-w64.org/) (AMD64 targets)
- [Clang](https://clang.llvm.org/) (ARM64 targets)
- [NASM](https://nasm.us/) (AMD64 targets)
- [NeBuild](https://github.com/nekernel-org/nebuild) (build system)

### **Building & Running**

```sh
git clone https://github.com/nekernel-org/nekernel.git
cd nekernel
./scripts/setup_x64_project.sh
./scripts/modules_ahci_x64.sh
./scripts/debug_ahci_x64.sh   # For debug generic AHCI target (QEMU)
```

---

## Structure

- `src/kernel/` — Core kernel source (scheduling, memory, VFS, drivers)
- `src/boot/` — Bootloader, platform bring-up, and early system code
- `src/libDDK/` — Driver Development Kit (DDK) and sample drivers
- `src/libSystem/` — Userland system call interface and runtime
- `src/launch/` — NeKernel Launch System
- `src/libMsg/` — NeKernel OpenMSG framework
- `public/tools/` — CLI tools (mkfs, fsck, open, manual, etc.)
- `public/frameworks/` — Userland frameworks (CoreFoundation, DiskImage, etc.)
- `docs/` — Specifications, design docs, and diagrams

---

## Security

- **Vulnerability Disclosure:**  
  Please report security issues privately via email or GitHub Security Advisories.

---

## Documentation

- [Documentation](https://docs.nekernel.org/)
- [Specifications](docs/tex/)

---

## Contributing

- Please run `format.sh` before committing (uses `.clang-format`).
- All contributions (code, docs, fuzzing, security) are welcome!

---

## Authors & Credits

- **Amlal El Mahrouss** — Lead Developer and Kernel Architect.
- [Full contributor list](https://github.com/nekernel-org/nekernel/graphs/contributors)

---

## License

This project is licensed under the [Apache-2.0 License](LICENSE).

---

<div align="center">
  <sub>
    &copy; 2024-2025 Amlal El Mahrouss & NeKernel contributors. Licensed under the Apache 2.0 license.
  </sub>
</div>
