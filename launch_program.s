	.file	"launch_program.c"
	.text
	.section	.rodata
.LC0:
	.string	"firefox"
.LC1:
	.string	"gedit"
.LC2:
	.string	"/usr/bin/firefox"
.LC3:
	.string	"Error launching browser\n"
.LC4:
	.string	"/usr/bin/gedit"
.LC5:
	.string	"Error launching editor\n"
.LC6:
	.string	"Launched browser with PID=%d\n"
.LC7:
	.string	"Launched editor with PID=%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC0(%rip), %rax
	movq	%rax, -48(%rbp)
	movq	$0, -40(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -32(%rbp)
	movq	$0, -24(%rbp)
	call	fork@PLT
	movl	%eax, -56(%rbp)
	cmpl	$0, -56(%rbp)
	jne	.L2
	leaq	-48(%rbp), %rax
	movl	$0, %edx
	movq	%rax, %rsi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	execve@PLT
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$24, %edx
	movl	$1, %esi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %eax
	jmp	.L5
.L2:
	call	fork@PLT
	movl	%eax, -52(%rbp)
	cmpl	$0, -52(%rbp)
	jne	.L4
	leaq	-32(%rbp), %rax
	movl	$0, %edx
	movq	%rax, %rsi
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	call	execve@PLT
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$23, %edx
	movl	$1, %esi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$1, %eax
	jmp	.L5
.L4:
	movl	-56(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-52(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC7(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
.L5:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L6
	call	__stack_chk_fail@PLT
.L6:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 14.2.1 20240910"
	.section	.note.GNU-stack,"",@progbits
