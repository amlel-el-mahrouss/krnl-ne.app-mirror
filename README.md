
<!-- Read Me of NeKernel -->

<div align="center">
  <h1>
    <b>The NeKernel System</b>
  </h1>
  <p>
    <a href="https://github.com/nekernel-org/nekernel/actions/workflows/boot-ahci-dev.yml/badge.svg"><img src="https://github.com/nekernel-org/nekernel/actions/workflows/boot-ahci-dev.yml/badge.svg" alt="CI"></a>
    <a href="https://github.com/nekernel-org/nekernel/actions/workflows/kernel-ahci-dev.yml/badge.svg"><img src="https://github.com/nekernel-org/nekernel/actions/workflows/kernel-ahci-dev.yml/badge.svg" alt="CI"></a>
    <a href="LICENSE"><img src="https://img.shields.io/badge/License-Apache--2.0-blue.svg" alt="License"></a>
  </p>
</div>

<img src="meta/media/demo.gif" alt="NeKernel Demo" width="1280"/>

## Getting Started

### **Requirements**

- [MinGW](https://www.mingw-w64.org/)
- [Clang](https://clang.llvm.org/)
- [NASM](https://nasm.us/)
- [NeBuild](https://github.com/nekernel-org/nebuild)
- CoreUtils
- [Git](https://git-scm.com/)
- [Nectar](https://github.com/nekernel-org/nectar)

### **Building & Running**

Please fork, and clone the repository. Then follow those steps:

```sh
git clone https://github.com/<your-username>/nekernel.git
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

- **Amlal El Mahrouss** — Lead and Kernel Architect.
- [Full contributor list](https://github.com/nekernel-org/nekernel/graphs/contributors)

---

## Citing

- Refer to [CITATION.cff](CITATION.cff)

---

## License

NeKernel is licensed under the [Apache-2.0 License](LICENSE).

---

<div align="center">
  <sub>
    &copy; 2023-2026 Amlal El Mahrouss & NeKernel Authors. Licensed under the Apache 2.0 license.
  </sub>
</div>
