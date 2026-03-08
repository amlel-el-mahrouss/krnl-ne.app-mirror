/**
 lang: asm
 compiler: gnu
 */

.text

/** @brief Sets up the stack frame from KernelTask's Start function. */
__ddk_start:	
#if defined(__DDK_AMD64__)
	
	ret
#endif	
