#include <efi-x86-instruct.h>
#include <efi-asm.h>

#define DECL(name) \
	"\t.globl " #name "\n\t" \
	#name ":\n"

#define RET "\tret\n"

asm(".text\n");
asm(
	DECL(asm_cli)
	"\tcli\n"
		RET
	);

asm(
	DECL(asm_lgdt)
	"\tlgdt (%rcx)\n"
		RET
	);

