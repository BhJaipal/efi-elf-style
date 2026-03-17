.extern kernel_main
.extern stack_top

.section .text

.global kernel_entry
kernel_entry:
	call kernel_main

	/* kernel_main should never return. Halt if it does. */
.halt_loop:
	hlt
	jmp .halt_loop
