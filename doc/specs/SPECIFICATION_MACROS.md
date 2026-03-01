===================================

# 0: General Information:

===================================

The NeKernel System uses macro to configure its compilation. Some of them shall not be defined at the same time to avoid run-time issues.

===================================

# 1: NeKernel/BootZ:

===================================

- `__nekernel_max_cores` -> Max SMP cores usable by NeKernel's scheduler.
- `__nekernel_boot_core_index` -> Index of the boot core (0, 1, or 3).
- `__nekernel_allow_non_nekernel_pe` -> Allow non-subsystem 17 to run on NeKernel.
- `__nekernel_halkit_include_processor` HALKit include directory for Processor.h
- `__nekernel_dma_pool_start` NeKernel DMA pool start.
- `__nekernel_dma_pool_size` NeKernel DMA pool size.
- `__nekernel_dma_best_align` NeKernel Best DMA align size.

===================================

# 2: KernelTest:

===================================

- `__KT_TEST_MAIN` -> KernelTest entrypoint symbol.
- `KT_TESTING_ENABLED` -> Program is running under testing constraints.

