.text
	.globl	.unsw.retaddr.1
	.globl	.unsw.retaddr.2
	.globl	.unsw.retaddr.3
	.globl	.unsw.retaddr.4
	.globl	.unsw.retaddr.5
	.globl	.unsw.retaddr.6
	.globl	.unsw.retaddr.7
	.globl	.unsw.retaddr.8
	.globl	.unsw.retaddr.9

	.type	.unsw.retid, @object
	.data
	.globl	.unsw.retid
	.p2align	4
.unsw.retid:
	.long	0
	.long	1
	.long	2
	.long	3
	.long	4
	.long	5
	.long	6
	.long	7
	.long	8
	.long	9
	.size	.unsw.retid, 40

	.type	.unsw.retaddr, @object
	.globl	.unsw.retaddr
	.bss
	.p2align	4
.unsw.retaddr:
	.zero	4194304
	.size	.unsw.retaddr, 4194304
	.type	.unsw.retid.cnt, @object
	.data
	.globl	.unsw.retid.cnt
	.p2align	4
.unsw.retid.cnt:
	.long	10
	.size	.unsw.retid.cnt, 4

	.section	.rodata
	.type	.unsw.retaddr.bak, @object
	.globl	.unsw.retaddr.bak
	.p2align	4
.unsw.retaddr.bak:
	.long	0
	.long	.unsw.retaddr.1
	.long	.unsw.retaddr.2
	.long	.unsw.retaddr.3
	.long	.unsw.retaddr.4
	.long	.unsw.retaddr.5
	.long	.unsw.retaddr.6
	.long	.unsw.retaddr.7
	.long	.unsw.retaddr.8
	.long	.unsw.retaddr.9
	.size	.unsw.retaddr.bak, 40

