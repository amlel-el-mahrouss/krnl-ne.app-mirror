<!-- Read Me of NeKernel -->

# 🍯 The NeKernel Module

![License](https://img.shields.io/badge/LICENSE-Apache--2.0-blue.svg?style=for-the-badge)
![GitHub Repo stars](https://img.shields.io/github/stars/ne-foss/ne-kernel?style=for-the-badge)

<a href="https://github.com/ne-foss/ne-kernel/actions/workflows/boot-ahci-dev.yml/badge.svg"><img src="https://github.com/ne-foss/ne-kernel/actions/workflows/boot-ahci-dev.yml/badge.svg" alt="CI 1"></a>
<a href="https://github.com/ne-foss/ne-kernel/actions/workflows/kernel-ahci-dev.yml/badge.svg"><img src="https://github.com/ne-foss/ne-kernel/actions/workflows/kernel-ahci-dev.yml/badge.svg?style=for-the-badge" alt="CI 2"></a>
<a href="https://github.com/ne-foss/ne-kernel/actions/workflows/libddk-build.yml/badge.svg"><img src="https://github.com/ne-foss/ne-kernel/actions/workflows/libddk-build.yml/badge.svg?style=for-the-badge" alt="CI 3"></a>
<a href="https://github.com/ne-foss/ne-kernel/actions/workflows/libsystem-build.yml/badge.svg"><img src="https://github.com/ne-foss/ne-kernel/actions/workflows/libsystem-build.yml/badge.svg?style=for-the-badge" alt="CI 4"></a>

## Getting Started

### **Requirements**

- [MinGW](https://www.mingw-w64.org/)
- [Clang](https://clang.llvm.org/)
- [NASM](https://nasm.us/)
- [NeBuild](https://github.com/ne-foss/nebuild)
- CoreUtils
- [Git](https://git-scm.com/)
- [Nectar](https://github.com/ne-foss/nectar)

### **Building & Running**

Please fork, and clone the repository. Then follow those steps:

```sh
git clone -j8 https://github.com/ne-foss/nekernel.git
cd nekernel
./scripts/setup_x64_project.sh
./scripts/modules_ahci_x64.sh
./scripts/debug_ahci_x64.sh   # For debug generic AHCI target (QEMU, UDF)
```

---

## Community

Join Ne.app's [discord](https://discord.gg/uD76Qweght) to contribute and chat with contributors.

## Documentation

- [Documentation](https://docs.src.nekernel.org/)

## License

NeKernel is licensed under the [Apache-2.0 License](LICENSE.txt).

---


<div align="center">
  <sub>
    &copy; 2022-2026 Amlal El Mahrouss. Licensed under the Apache 2.0 license.
  </sub>
</div>
