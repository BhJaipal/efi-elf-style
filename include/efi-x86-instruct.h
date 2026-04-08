#ifndef EFI_X86_INSTRUCT
#define EFI_X86_INSTRUCT

#ifndef EFI_ELF_TYPES
#include "types.h"
#endif /* ifndef EFI_ELF_TYPES */

typedef struct {
	uint64 rax, rdi;
	uint64 rsi, rdx;
	uint64 rcx, r10;
	uint64 r8,  r9;
} reg_t;

#define NAKED __attribute__((naked))


#endif /* ifndef EFI_X86_INSTRUCT */
