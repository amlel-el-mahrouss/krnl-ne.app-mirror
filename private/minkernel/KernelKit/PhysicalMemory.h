#ifndef KERNELKIT_PHYSICALMEMORY_H
#define KERNELKIT_PHYSICALMEMORY_H

#include <NeKit/Config.h>
#include <hint/CompilerHint.h>

#ifdef __NE_AMD64__
#include <HALKit/AMD64/Paging.h>
#endif  // __NE_AMD64__

namespace Ne::Kernel::HAL {
/// @brief Hand a physical region to the frame allocator.
Void pmm_init(UIntPtr base, SizeT sz);

/// @brief Take one zeroed frame, 0 when out of memory.
_Output UIntPtr pmm_alloc_frame(Void);

/// @brief Give a frame back.
Void pmm_free_frame(UIntPtr frame);

/// @brief Frames still available.
_Output SizeT pmm_free_frames(Void);

/// @brief Frames handed out.
_Output SizeT pmm_used_frames(Void);
}  // namespace Ne::Kernel::HAL

#endif /* ifndef KERNELKIT_PHYSICALMEMORY_H */
