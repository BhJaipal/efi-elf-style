.global kernel_entry
.section .text

kernel_entry:
	cli
	leaq stack_top, %rax
	addq $-16, %rsp
	xorq %rbp, %rbp
	call kernel_main

	/* kernel_main should never return. Halt if it does. */
.halt_loop:
	# hlt
	jmp .halt_loop

	.section .note.GNU-stack,"",@progbits

.extern kernel_main
.extern stack_top
