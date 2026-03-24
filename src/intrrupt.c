#include <efi-x86-instruct.h>

void intrupt(uint64 intrrupt_flag, reg_t *registers) {
	asm (
		"mov %0, %%rdx\n"
		"mov %1, %%rsi\n"
		"mov %2, %%rdi\n"
		"mov %3, %%rax\n"
		::
			"r"(registers->rdx),
			"r"(registers->rsi),
			"r"(registers->rdi),
			"r"(registers->rax)
	);
}
