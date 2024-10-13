	.file	"newprog_program.c"
	.text
	.section	.rodata
.LC0:
	.string	"Tee-hee!"
.LC1:
	.string	"newprog_arg1"
.LC2:
	.string	"newprog_arg2"
.LC3:
	.string	"USER=abrakadabra"
.LC4:
	.string	"HOME=/home/abrakadabra"
.LC5:
	.string	"Old PID: %d\n"
.LC6:
	.string	"./newprog"
.LC7:
	.string	"Error\n"
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
	subq	$80, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC0(%rip), %rax
	movq	%rax, -48(%rbp)
	leaq	.LC1(%rip), %rax
	movq	%rax, -40(%rbp)
	leaq	.LC2(%rip), %rax
	movq	%rax, -32(%rbp)
	movq	$0, -24(%rbp)
	leaq	.LC3(%rip), %rax
	movq	%rax, -80(%rbp)
	leaq	.LC4(%rip), %rax
	movq	%rax, -72(%rbp)
	movq	$0, -64(%rbp)
	call	getpid@PLT
	movl	%eax, %esi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-80(%rbp), %rdx
	leaq	-48(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	call	execve@PLT
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$6, %edx
	movl	$1, %esi
	leaq	.LC7(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L3
	call	__stack_chk_fail@PLT
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 14.2.1 20240910"
	.section	.note.GNU-stack,"",@progbits
