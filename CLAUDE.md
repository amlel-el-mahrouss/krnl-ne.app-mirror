# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build System

NeKernel uses a combination of **NeBuild** (JSON-based build tool) and **Make** for building different components.

### Initial Setup

```sh
# Setup x64 project (builds libSystem, DDK, and boot components)
./scripts/setup_x64_project.sh

# Setup ARM64 project
./scripts/setup_arm64_project.sh
```

### Building & Running

```sh
# Build boot modules (SysChk, BootNet)
./scripts/modules_ahci_x64.sh

# Build and run with AHCI support in QEMU (debug mode)
./scripts/debug_ahci_x64.sh

# Build and run with ATA PIO support
./scripts/debug_ata_x64.sh

# Release builds
./scripts/release_ahci_x64.sh
```

### Component-Specific Builds

```sh
# Build kernel only (from src/kernel/)
make -f amd64-desktop.make all

# Build bootloader (from src/boot/)
make -f amd64-desktop.make efi
make -f amd64-desktop.make epm-img

# Build libSystem (from src/libSystem/)
nebuild libSystem.json

# Build DDK (from src/ddk/)
nebuild ddk.json
```

### Testing

Tests use the KernelTest.fwrk framework:

```sh
# Test files are in test/<component>_test/
# Example: test/kout_test/kout.test.cc

# Test structure:
KT_DECL_TEST(TestName, []() -> bool { /* test logic */ });
KT_TEST_MAIN() { KT_RUN_TEST(TestName); return KT_TEST_SUCCESS; }
```

### Code Formatting

**Always run before committing:**

```sh
./format.sh
```

Uses `.clang-format` config (Google style, 100 column limit, 2-space indent).

## Architecture Overview

NeKernel is a modern microkernel-style OS with clean separation between kernel, drivers (DDK), and userland (libSystem).

### Boot Flow

```
Firmware (UEFI/EFI) → Bootloader (BootKit) → Handover Protocol → Kernel Init
```

**Handover Protocol** (`FirmwareKit/Handover.h`):
- Passes `BootInfoHeader` with kernel/libSystem addresses, 4GB memory bitmap, GOP framebuffer, firmware tables
- Magic: `0xBADCC`, Version: `0x0117`

### Core Architecture

**Three-Layer Design:**

```
Userland (libSystem.dll)
    ↓ System Calls (hash-based routing)
Kernel (DDK/DriverKit)
    ↓ HAL abstraction
Hardware
```

### Kernel Organization ("Kit" Modules)

Located in `src/kernel/`:

- **NeKit**: Foundation (ErrorOr<T>, Ref, Array, Stream, JSON/TOML, Vettable concept)
- **KernelKit**: Schedulers (ProcessScheduler, UserProcessScheduler, KernelTaskScheduler), process trees (Red-Black tree), PCI/DMA
- **HALKit**: Hardware abstraction (AMD64, ARM64, PowerPC, RISC-V)
- **FSKit**: Filesystems (NeFS, Ext2+IFS, OpenHeFS)
- **StorageKit**: Device drivers (AHCI, ATA, SCSI, NVME)
- **DeviceMgr**: Template-based device interface `IDevice<T>` with `IOBuf<T>` for DMA
- **DriveMgr**: Drive abstraction with `DriveTrait` structs
- **FileMgr**: VFS abstraction `IFilesystemMgr`, `FileStream<Encoding>` template
- **FirmwareKit**: EFI/UEFI, GPT/EPM partition schemes
- **NetworkKit**: IPC protocol (128-bit addressing: ProcessID:Team, 6KB messages, CRC32 validation)
- **SignalKit**: Signal generation (SIGKILL, SIGPAUSE, SIGSEGV, etc.)
- **CodeMgr**: Executable formats (PE, PEF, PE32, XCOFF, ZXD)
- **HeapMgr, SwapKit, DmaKit**: Memory management
- **UserMgr**: User/privilege management (Ring 1, 2, 3)
- **TraceSrv**: Tracing and debugging

### System Calls (libSystem API)

Located in `src/libSystem/SystemKit/System.h`. Uses reference types (`Ref` with hash+pointer).

Key APIs:
- **File I/O**: `IoOpenFile`, `IoReadFile`, `IoWriteFile`, `IoSeekFile`, `IoCtrlFile`
- **Process**: `RtlSpawnProcess`, `RtlExitProcess`, `RtlSpawnIB` (threads)
- **Memory**: `MmCreateHeap`, `MmDestroyHeap`, `MmCopyMemory`
- **Threading**: `ThrCreateThread`, `ThrYieldThread`, `ThrJoinThread`
- **Filesystem**: `FsCopy`, `FsMove`, `FsCreateDir`, `IfsMount`, `DrvMountDrive`
- **Dynamic Loading**: `LdrOpenDylibHandle`, `LdrGetDylibSymbolFromHandle`
- **Power**: `PwrReadCode`, `PwrSendCode` (shutdown/reboot)
- **Scheduler**: `SchedSetAffinity`, `SchedFireSignal`, `SchedGetCurrentProcessID`
- **Events**: `EvtAddListener`, `EvtDispatchEvent`

### DDK (Device Driver Kit)

Located in `src/libDDK/DriverKit/`:

**C API**:
- `ke_call_dispatch()` - kernel dispatch
- `ke_set_syscall(slot, function_pointer)` - register syscalls
- `kalloc/kfree` - kernel heap
- `DDK_DEVICE` struct with function pointers (read/write/open/close/seek)
- `DDK_OBJECT_MANIFEST` for exposing kernel objects via `ke_get_obj`/`ke_set_obj`

**C++ Wrapper**: `IDriverBase` concept with `IsValidDriver` template concept

### Error Handling

Uses `ErrorOr<T>` template pattern (`src/kernel/NeKit/ErrorOr.h`):

```cpp
ErrorOr<T> result = someFunction();
if (result) {
  auto value = result.Value();
} else {
  auto error = result.Error(); // kErrorFileNotFound, kErrorHeapOutOfMemory, etc.
}
```

37+ error codes defined in `KernelKit/KPC.h`.

## Key Architectural Patterns

### Vettable Concept (New API)

Replaces old `IsAcceptable` concept for marking schedulable vs non-schedulable entities:

```cpp
// Mark a type as schedulable
#define NE_VETTABLE static constexpr BOOL kVettable = YES

// Mark as non-schedulable (kernel-only)
#define NE_NON_VETTABLE static constexpr BOOL kVettable = NO

// Concept usage
template <class Type>
concept IsVettable = requires(Type) { (Type::kVettable); };
```

**UserProcess** is `NE_VETTABLE`, **KernelTask** is `NE_NON_VETTABLE`.

### Process Scheduling

Multi-threaded scheduler with Team model (`KernelKit/UserProcessScheduler.h`):
- **Process Teams**: Low, Mid, High, RealTime priority
- **Affinities**: VeryLow, Low, Standard, High, VeryHigh, RealTime (nanosecond-level time slicing)
- **Process Status**: Starting, Running, Killed, Frozen, Finished
- **Subsystems**: Security, User, Service, Driver, Kernel
- **Resource Trees**: Red-Black tree for heap/file/special resources per process
- **Limits**: 128GB memory limit, 8KB stack size per process

### NeFS Filesystem

Custom filesystem inspired by HFS+ (`FSKit/NeFS.h`):
- **Fork Model**: Separate data and resource forks
- **Catalog-Based**: Similar to Mac HFS catalog structure
- **Buddy Allocation**: With sibling tracking
- **Catalog Kinds**: File, Directory, Alias, Executable, Device, etc.
- **Framework Convention**: `.fwrk/`, `.app/` directory suffixes

### IPC (Inter-Process Communication)

128-bit addressing model (`NetworkKit/IPC.h`):
- **Address**: ProcessID (64-bit) + Team (64-bit)
- **Message**: 6KB payload max, CRC32 validation
- **Endianness**: Configurable (LE/BE/mixed)

## Important Files for Understanding Architecture

1. `src/kernel/FirmwareKit/Handover.h` - Boot protocol and firmware handover
2. `src/kernel/KernelKit/UserProcessScheduler.h` - Process scheduling model
3. `src/kernel/KernelKit/DriveMgr.h` - Device abstraction layer
4. `src/kernel/FSKit/NeFS.h` - Custom filesystem implementation
5. `src/libSystem/SystemKit/System.h` - Userland system call API
6. `src/kernel/NeKit/Vettable.h` - Type concept system for schedulability
7. `src/kernel/NetworkKit/IPC.h` - Inter-process communication
8. `src/kernel/NeKit/ErrorOr.h` - Error handling pattern

## Development Notes

- **Language**: C++20 with concepts, templates, and no exceptions/RTTI
- **Compiler**: MinGW GCC (x86_64-w64-mingw32-g++), Clang for some targets
- **Assembler**: NASM
- **Build Tools**: NeBuild (custom JSON-based) + Make
- **Supported ISAs**: AMD64, ARM64, PowerPC, RISC-V
- **Partition Schemes**: EPM (custom), GPT, MBR
- **Subsystem**: Windows subsystem 17 for libSystem.dll

## Common Patterns to Follow

### Template-Based Abstractions

```cpp
// Device abstraction
IDevice<T> → IDevice<IMountpoint*>

// File streams with encoding
FileStream<Encoding, FSClass>

// DMA buffers
IOBuf<T>
```

### Reference Types

System calls use reference-based handles (hash + pointer) instead of raw integers for type safety.

### Concept-Based Constraints

- `IsVettable` - Marks schedulable types
- `IsValidDriver` - Validates driver implementations

## Tools & Utilities

Located in `tools/` and `public/tools/`:

- **mkfs.hefs.cc** - Create HeFS filesystems
- **chk.hefs.cc** - Check/repair HeFS filesystems
- **mk_img.py** - Create disk images
- **mk_fwrk.py** - Package frameworks (.fwrk)
- **mk_app.py** - Package applications (.app)

## Debugging

```sh
# Run with GDB
./gdb.sh

# Run with LLDB
./scripts/lldb.sh
```

## CI/CD

```sh
# Kernel CI build (x64)
./scripts/kernel_ci_x64.sh
```

GitHub Actions run on commits:
- `boot-ahci-dev.yml` - Boot tests with AHCI
- `kernel-ahci-dev.yml` - Kernel tests with AHCI
