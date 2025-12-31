===================================

# 0: General Information

===================================

- ABI and Format: PEF/PE32+.
- Target: NeKernel.

===================================

# 1: The specification:

===================================

The NeKernel framework system (`.fwrk`) provides a standardized structure for creating modular, reusable libraries and components. Frameworks are self-contained packages that include headers, source code, metadata, and configuration for compilation and deployment within the NeKernel ecosystem.

==================================

# 2: Framework Directory Structure:

==================================

Each framework follows the standardized directory layout below:

```
<FrameworkName>.fwrk/
├── <FrameworkName>.json          # Framework manifest and build configuration
├── headers/                       # Public API headers
│   ├── *.h                       # Public header files (C++ headers)
│   └── ...
├── src/                          # Implementation source files
│   ├── *.cc                      # C++ implementation files
│   ├── DylibMain.cc              # Dynamic library entry point (optional)
│   └── ...
├── xml/                          # Framework metadata and properties
│   └── app.xml                   # Framework property list
├── dist/                         # Build output directory
│   └── lib<FrameworkName>.fwrk.dylib  # Compiled dynamic library
└── .keep                         # Placeholder file
```

================================

# 3: Component Specifications

================================

### 1. Framework Manifest (`<FrameworkName>.json`)

The JSON manifest file defines the build configuration, compilation flags, and metadata for the framework.

**Required Fields:**

- **`compiler_path`** (string): Path to the C++ compiler executable
  - Example: `"clang++"`, `"x86_64-w64-mingw32-g++"`

- **`compiler_std`** (string): C++ standard version
  - Example: `"c++20"`, `"c++17"`

- **`headers_path`** (array): Array of header search paths (relative to framework)
  - Include paths for finding dependencies and kernel interfaces
  - Example: `["./", "../../../src/kernel", "../../../public/frameworks/", "../../../src/"]`

- **`sources_path`** (array): Array of source file globs to compile
  - Example: `["src/*.cc"]`

- **`output_name`** (string): Path and filename for compiled library output
  - Example: `"./dist/libCoreFoundation.fwrk.dylib"`

**Optional Fields:**

- **`compiler_flags`** (array): Custom compilation flags
  - Example: `["-ffreestanding", "-shared", "-fno-rtti", "-fno-exceptions"]`

- **`cpp_macros`** (array): C++ preprocessor macro definitions
  - Example: `["kCFVersion=0x0100", "__NE_AMD64__", "__CF_64BIT__"]`

**Example Manifest:**
```json
{
    "compiler_path": "x86_64-w64-mingw32-g++",
    "compiler_std": "c++20",
    "headers_path": ["../", "./", "../../../dev", "../../../src/kernel"],
    "sources_path": ["src/*.cc"],
    "output_name": "./dist/libCoreFoundation.fwrk.dylib",
    "compiler_flags": [
      "-ffreestanding",
      "-shared",
      "-fno-rtti",
      "-fno-exceptions",
      "-Wl,--subsystem=17"
    ],
    "cpp_macros": [
      "kCFVersion=0x0100",
      "kCFVersionHighest=0x0100",
      "kCFVersionLowest=0x0100",
      "__NE_AMD64__",
      "__CF_64BIT__"
    ]
}
```

### 2. Public Headers Directory (`headers/`)

Contains all public-facing C++ header files that define the framework's API.

**Conventions:**
- File extension: `.h` (not `.hpp`)
- Namespace: Typically matches framework name (e.g., `CF::` for CoreFoundation)
- Include guards: Use `#pragma once` for header protection
- Copyright headers: Include NeKernel copyright notice at the top

**Typical Headers:**
- Main header providing overall framework interface
- Specialized headers for different components
- Type definitions and class declarations
- Exception/error definitions

### 3. Source Implementation Directory (`src/`)

Contains C++ implementation files for the framework.

**Key Files:**

- **`DylibMain.cc`** (Optional):
  - Entry point for dynamic library initialization
  - Contains `_DylibAttach(SInt32 argc, Char* argv[])` function
  - Returns status code (0 for success, EXIT_FAILURE otherwise)
  
  **Example:**
  ```cpp
  #include <libSystem/SystemKit/System.h>
  
  SInt32 _DylibAttach(SInt32 argc, Char* argv[]) {
      // Initialization code here
      return EXIT_FAILURE;  // Change based on initialization result
  }
  ```

- **Implementation Files** (`*.cc`):
  - Correspond to public headers
  - Contain class definitions and function implementations
  - Include necessary system and framework headers

### 4. Metadata Directory (`xml/`)

Contains XML-based property list files for framework metadata.

**`app.xml` Structure:**
- PropertyList format for framework configuration
- Typically includes:
  - `LibraryName`: Framework name (string)
  - `CacheLibs`: Whether to cache library (boolean)
  - Other framework-specific properties

**Example:**
```xml
<PropertyList/>
<PLEntry Type="CFString" Name="LibraryName" Len="255" Value="CoreFoundation" />
<PLEntry Type="BOOL" Name="CacheLibs" Value="YES" />
```

### 5. Build Output Directory (`dist/`)

Auto-generated directory containing compiled framework binaries.

- **Output file**: `lib<FrameworkName>.fwrk.dylib`
- Created during build process via `mkfwrk.py`
- Not committed to version control

## Built-in Frameworks

The NeKernel project includes the following standard frameworks:

### CoreFoundation.fwrk
**Purpose:** Core data structures and utilities foundation
**Provides:**
- Reference types (CFRef)
- String handling (CFString)
- Collections (Arrays, Dictionaries)
- Geometric types (CFPoint, CFRect, CFColor)
- Object system (CFObject, CFProperty)

### DiskImage.fwrk
**Purpose:** Disk image management and handling

### KernelTest.fwrk
**Purpose:** Testing framework for kernel components

### LaunchHelpers.fwrk
**Purpose:** Helper functions for system launch and initialization

## Framework Creation

### Using mkfwrk.py

The `mkfwrk.py` tool automates framework creation:

```bash
python3 tools/mkfwrk.py <framework_name>
```

This generates:
1. Complete directory structure
2. Template `<framework_name>.json` manifest
3. Skeleton `DylibMain.cc` entry point
4. Template `xml/app.xml` metadata

**Generated Structure:**
```
<framework_name>.fwrk/
├── <framework_name>.json
├── headers/
│   └── .keep
├── src/
│   ├── .keep
│   └── DylibMain.cc
├── xml/
│   └── app.xml
└── .keep
```

## Compilation and Linking

### Build Process

1. **Configuration Phase**: Read and parse `<framework_name>.json`
2. **Compilation Phase**: Compile source files with specified compiler and flags
3. **Linking Phase**: Link object files into dynamic library
4. **Output Phase**: Place compiled binary in `dist/` directory

### Dependencies

- Frameworks can depend on:
  - System libraries (via SystemKit)
  - Other frameworks (via headers_path)
  - Kernel components (via kernel paths in headers_path)

### Linking Order

When building applications that use frameworks:
1. Frameworks are linked in dependency order
2. CoreFoundation is typically a base dependency
3. Framework paths and library names must match manifest output names

## Naming Conventions

- **Framework directories**: `<Name>.fwrk` (PascalCase + .fwrk suffix)
- **JSON manifest**: `<Name>.json` (matches framework directory name)
- **Output library**: `lib<Name>.fwrk.dylib`
- **Namespaces**: Two-letter abbreviations (e.g., `CF`, `DI`, `KT`, `LH`)
- **Macros**: `k<ComponentName>` prefix (e.g., `kCFVersion`)

## Version Specification

Frameworks should include version information via preprocessor macros:

- **`k<Framework>Version`**: Current framework version (e.g., `0x0100` for v1.0)
- **`k<Framework>VersionHighest`**: Highest compatible version
- **`k<Framework>VersionLowest`**: Lowest compatible version

**Version Format**: Hexadecimal (e.g., `0x0100` = 1.0, `0x0201` = 2.1)

## Architecture Support

Frameworks can be compiled for multiple architectures:

- **x86_64** (amd64): 64-bit x86/AMD64 architecture
- **ARM64**: 64-bit ARM architecture
- **RISC-V 64**: RISC-V 64-bit architecture
- **PowerPC 64**: PowerPC 64-bit architecture

Architecture is typically controlled via:
- Compiler selection in manifest
- C++ macros (`__NE_AMD64__`, `__NE_ARM64__`, etc.)

## Best Practices

1. **Header Organization**: Keep headers minimal; place implementation in `.cc` files
2. **Namespace Usage**: Encapsulate all public APIs in framework namespace
3. **Dependencies**: Minimize external dependencies; prefer kernel APIs
4. **Configuration**: Use `.json` manifest for all build-time configuration
5. **Documentation**: Include copyright headers and documentation comments in code
6. **Error Handling**: Define framework-specific error codes and status returns
7. **Testing**: Provide test cases using KernelTest.fwrk when applicable

## Integration with Applications

Applications that use frameworks:

1. **Include Headers**: `#include <Framework/Header.h>`
2. **Link Framework**: Add framework library to linker flags
3. **Initialize**: Call framework initialization if provided
4. **Handle Errors**: Check return codes and handle framework exceptions

## File Permissions

- Framework files: Standard text (`.h`, `.cc`, `.json`, `.xml`)
- Build artifacts: Auto-generated and read-only during runtime
- Framework package: Should be distributable as single `.fwrk` directory

