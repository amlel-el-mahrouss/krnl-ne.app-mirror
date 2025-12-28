===================================

# 0: General Information:

===================================

- ABI and Format: PEF/PE32+.
- Kernel architecture: Portable hybrid Kernel.
- Used Languages: C++, and Assembly Assembly (AMD64, X64000, X86S, ARM64, POWER, RISCV)
- 32-bit is not supported as of 12/25/2025.

===================================

# 1: The Kernel (NeKernel)

===================================

- Drive/Device Abstraction.
- SMP, Preemptive Multi Threading.
- Separation of Files/Devices.
- Networking Support.
- Hardware Abstraction Layer.
- Native Filesystem support (NeFS, FAT32 and ffs2).
- Program Loaders interfaces.
- TLS (Thread Local Storage) support.
- BinaryMutex, Locks, Timers.
- Canary mechanisms.
- Dynamic Loader.
- Cross Platform.
- Permission Selectors.
- Modular and Security focused.

===================================

# 2: The Filesystem (NeFS, or OpenHeFS)

===================================

- Catalog object with associated forks.
- Large storage support.
- Long file names.
- UNIX path style.
- Can be formated under 8mb.

==================================

# 3: Common conventions:

==================================

- Kernel -> ke_init_x
- RunTime -> rt_copy_mem
- Hal -> hal_foo_bar
- Class methods -> Class::FooBar
- Internals function shall be formated as such: (hali, rtli, rti...)

===================================

# 4: The Bootloader (BootZ)

===================================

- Capable of booting from a network drive.
- Loads a PE file which is the Kernel, or any modules.
- Sanity checks, based on the number of sections.
- Handover compliant.
- Does check for a valid partition (useful in the case of recovering)
- Modular, and Security focused.

===================================

# 5: The IFS

==================================

- Filesystem to mountpoint interface abstraction.
- Multi-drive support (A, B, C, D indices).
- Packet-based I/O operations.
- Separation of read/write operations per drive.
