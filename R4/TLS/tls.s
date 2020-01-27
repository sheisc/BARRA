	.text
	.file	"tls.c"
	.type	barra_ranval,@object    # @barra_ranval
	.section	.tbss,"awT",@nobits
	.globl	barra_ranval
	.p2align	2
barra_ranval:
	.long	0                       # 0x0
	.size	barra_ranval, 4


	.ident	"clang version 7.0.0 (tags/RELEASE_700/final)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
