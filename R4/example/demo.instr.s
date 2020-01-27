	.text
	.file	"llvm-link"
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
	popq	-16777216(%rsp)
	subq	$8, %rsp
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	$0, -8(%rbp)
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movb	$0, %al
	movq	$1, %rax
	callq	do_request
.unsw.retaddr.1:
	xorl	%eax, %eax
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	addq	$8, %rsp
	pushq	-16777216(%rsp)
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.globl	read_and_echo           # -- Begin function read_and_echo
	.p2align	4, 0x90
	.type	read_and_echo,@function
read_and_echo:                          # @read_and_echo
	.cfi_startproc
	addq	.unsw.randomval, %rax
	andq	$1048575, %rax
	movq	%rax, -16777216(%rsp)
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$1024, %rsp             # imm = 0x400
	leaq	-1024(%rbp), %rdi
	callq	gets
.unsw.retaddr.2:
	leaq	-1024(%rbp), %rdi
	movb	$0, %al
	callq	printf
.unsw.retaddr.3:
	movabsq	$.L.str, %rdi
	movb	$0, %al
	callq	printf
.unsw.retaddr.4:
	addq	$1024, %rsp             # imm = 0x400
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	movq	-16777216(%rsp), %r11
	subq	.unsw.randomval, %r11
	andq	$1048575, %r11
	addq	$8, %rsp
	jmp	*.unsw.retaddr(,%r11,8)
.Lfunc_end1:
	.size	read_and_echo, .Lfunc_end1-read_and_echo
	.cfi_endproc
                                        # -- End function
	.globl	do_request              # -- Begin function do_request
	.p2align	4, 0x90
	.type	do_request,@function
do_request:                             # @do_request
	.cfi_startproc
	addq	.unsw.randomval, %rax
	andq	$1048575, %rax
	movq	%rax, -16777216(%rsp)
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	$3, -4(%rbp)
.LBB2_1:                                # =>This Inner Loop Header: Depth=1
	cmpl	$0, -4(%rbp)
	jle	.LBB2_3
# %bb.2:                                #   in Loop: Header=BB2_1 Depth=1
	movq	$5, %rax
	callq	read_and_echo
.unsw.retaddr.5:
	movl	-4(%rbp), %eax
	addl	$-1, %eax
	movl	%eax, -4(%rbp)
	jmp	.LBB2_1
.LBB2_3:
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	movq	-16777216(%rsp), %r11
	subq	.unsw.randomval, %r11
	andq	$1048575, %r11
	addq	$8, %rsp
	jmp	*.unsw.retaddr(,%r11,8)
.Lfunc_end2:
	.size	do_request, .Lfunc_end2-do_request
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"\n"
	.size	.L.str, 2


	.ident	"clang version 7.0.0 (tags/RELEASE_700/final)"
	.ident	"clang version 7.0.0 (tags/RELEASE_700/final)"
	.section	".note.GNU-stack","",@progbits
	.addrsig

.text
	.addrsig_sym .unsw.retaddr
	.addrsig_sym .unsw.retid
	.addrsig_sym .unsw.randomval

.text
	.globl	.unsw.retaddr.1
	.type	.unsw.retaddr.1,@function
	.globl	.unsw.retaddr.2
	.type	.unsw.retaddr.2,@function
	.globl	.unsw.retaddr.3
	.type	.unsw.retaddr.3,@function
	.globl	.unsw.retaddr.4
	.type	.unsw.retaddr.4,@function
	.globl	.unsw.retaddr.5
	.type	.unsw.retaddr.5,@function
