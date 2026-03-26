#ifndef EFI_ASM
#define EFI_ASM

struct __attribute__((packed)) GDTDescriptor {
	unsigned short      size;
	unsigned long long  offset;
};

extern void asm_cli();

extern void asm_lgdt(struct GDTDescriptor gdt_ptr);

#endif /* ifndef EFI_ASM */
