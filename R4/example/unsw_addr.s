.text
	.globl	.unsw.retaddr.1
	.globl	.unsw.retaddr.2
	.globl	.unsw.retaddr.3
	.globl	.unsw.retaddr.4
	.globl	.unsw.retaddr.5

	.type	.unsw.retaddr, @object
	.globl	.unsw.retaddr
	.bss
	.p2align	4
.unsw.retaddr:
	.zero	8388608
	.size	.unsw.retaddr, 8388608

	.type	.unsw.retid.cnt, @object
	.data
	.globl	.unsw.retid.cnt
	.p2align	4
.unsw.retid.cnt:
	.quad	6
	.size	.unsw.retid.cnt, 8

	.type	.unsw.randomval, @object
	.globl	.unsw.randomval
	.bss
	.p2align	12
.unsw.randomval:
	.zero	4096
	.size	.unsw.randomval, 4096

	.section	.rodata
	.type	.unsw.retaddr.bak, @object
	.globl	.unsw.retaddr.bak
	.p2align	4
.unsw.retaddr.bak:
	.quad	0
	.quad	.unsw.retaddr.1
	.quad	.unsw.retaddr.2
	.quad	.unsw.retaddr.3
	.quad	.unsw.retaddr.4
	.quad	.unsw.retaddr.5
	.size	.unsw.retaddr.bak, 48

