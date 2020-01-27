	.text
	.file	"server.c"
	.file	1 "/home/barra/src/R4/echo" "server.c"
	.file	2 "/home/barra/src/R4/echo" "/usr/include/bits/socket_type.h"
	.file	3 "/home/barra/src/R4/echo" "/usr/include/netinet/in.h"
	.file	4 "/home/barra/src/R4/echo" "/usr/include/stdint.h"
	.file	5 "/home/barra/src/R4/echo" "/usr/include/bits/sockaddr.h"
	.file	6 "/home/barra/src/R4/echo" "/usr/include/bits/socket.h"
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
.Lfunc_begin0:
	.loc	1 83 0                  # server.c:83:0
	.cfi_sections .debug_frame
	.cfi_startproc
	popl	-16777216(%esp)
	subl	$4, %esp
# %bb.0:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset %ebp, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register %ebp
	pushl	%esi
	subl	$148, %esp
	.cfi_offset %esi, -12
	movl	12(%ebp), %eax
	movl	8(%ebp), %ecx
	movl	$0, -8(%ebp)
.Ltmp0:
	.loc	1 88 10 prologue_end    # server.c:88:10
	cmpl	$2, 8(%ebp)
.Ltmp1:
	.loc	1 88 5 is_stmt 0        # server.c:88:5
	movl	%eax, -76(%ebp)         # 4-byte Spill
	movl	%ecx, -80(%ebp)         # 4-byte Spill
	je	.LBB0_2
# %bb.1:
.Ltmp2:
	.loc	1 89 37 is_stmt 1       # server.c:89:37
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	.loc	1 89 3 is_stmt 0        # server.c:89:3
	leal	.L.str, %ecx
	movl	%ecx, (%esp)
	movl	%eax, 4(%esp)
	calll	printf
	.loc	1 90 3 is_stmt 1        # server.c:90:3
	movl	$4294967295, %ecx       # imm = 0xFFFFFFFF
	movl	%eax, -84(%ebp)         # 4-byte Spill
	movl	%ecx, %eax
	addl	$148, %esp
	popl	%esi
	popl	%ebp
	.cfi_def_cfa %esp, 4
	addl	$4, %esp
	pushl	-16777216(%esp)
	retl
.Ltmp3:
.LBB0_2:
	.cfi_def_cfa %ebp, 8
	.loc	1 93 14                 # server.c:93:14
	movl	12(%ebp), %eax
	movl	4(%eax), %eax
	.loc	1 93 9 is_stmt 0        # server.c:93:9
	movl	%eax, (%esp)
	calll	atoi
	.loc	1 93 7                  # server.c:93:7
	movl	%eax, -12(%ebp)
	.loc	1 94 2 is_stmt 1        # server.c:94:2
	leal	.L.str.1, %eax
	movl	%eax, (%esp)
	calll	printf
	.loc	1 95 2                  # server.c:95:2
	leal	.L.str.2, %ecx
	movl	%ecx, (%esp)
	leal	sbuf, %ecx
	movl	%ecx, 4(%esp)
	leal	fptr, %ecx
	movl	%ecx, 8(%esp)
	movl	%eax, -88(%ebp)         # 4-byte Spill
	calll	printf
.Ltmp4:
	.loc	1 100 18                # server.c:100:18
	movl	$2, (%esp)
	movl	$1, 4(%esp)
	movl	$6, 8(%esp)
	movl	%eax, -92(%ebp)         # 4-byte Spill
	calll	socket
	.loc	1 100 16 is_stmt 0      # server.c:100:16
	movl	%eax, -16(%ebp)
	.loc	1 100 61                # server.c:100:61
	cmpl	$0, %eax
.Ltmp5:
	.loc	1 100 5                 # server.c:100:5
	jge	.LBB0_4
# %bb.3:
.Ltmp6:
	.loc	1 101 3 is_stmt 1       # server.c:101:3
	leal	.L.str.3, %eax
	movl	%eax, (%esp)
	calll	do_error
.Ltmp7:
.LBB0_4:
	.loc	1 0 3 is_stmt 0         # server.c:0:3
	xorl	%eax, %eax
	.loc	1 104 9 is_stmt 1       # server.c:104:9
	leal	-32(%ebp), %ecx
	.loc	1 104 2 is_stmt 0       # server.c:104:2
	movl	%ecx, (%esp)
	movl	$0, 4(%esp)
	movl	$16, 8(%esp)
	movl	%eax, -96(%ebp)         # 4-byte Spill
	calll	memset
	xorl	%ecx, %ecx
	.loc	1 105 23 is_stmt 1      # server.c:105:23
	movw	$2, -32(%ebp)
	.loc	1 106 30                # server.c:106:30
	movl	$0, (%esp)
	movl	%eax, -100(%ebp)        # 4-byte Spill
	movl	%ecx, -104(%ebp)        # 4-byte Spill
	calll	htonl
	.loc	1 106 28 is_stmt 0      # server.c:106:28
	movl	%eax, -28(%ebp)
	.loc	1 107 29 is_stmt 1      # server.c:107:29
	movl	-12(%ebp), %eax
	movw	%ax, %dx
	.loc	1 107 23 is_stmt 0      # server.c:107:23
	movzwl	%dx, %eax
	movl	%eax, (%esp)
	calll	htons
	.loc	1 107 21                # server.c:107:21
	movw	%ax, -30(%ebp)
.Ltmp8:
	.loc	1 109 11 is_stmt 1      # server.c:109:11
	movl	-16(%ebp), %ecx
	.loc	1 109 22 is_stmt 0      # server.c:109:22
	leal	-32(%ebp), %esi
	.loc	1 109 6                 # server.c:109:6
	movl	%ecx, (%esp)
	movl	%esi, 4(%esp)
	movl	$16, 8(%esp)
	calll	bind
	.loc	1 109 72                # server.c:109:72
	cmpl	$0, %eax
.Ltmp9:
	.loc	1 109 6                 # server.c:109:6
	jge	.LBB0_6
# %bb.5:
.Ltmp10:
	.loc	1 110 3 is_stmt 1       # server.c:110:3
	leal	.L.str.4, %eax
	movl	%eax, (%esp)
	calll	do_error
.Ltmp11:
.LBB0_6:
	.loc	1 113 13                # server.c:113:13
	movl	-16(%ebp), %eax
	.loc	1 113 6 is_stmt 0       # server.c:113:6
	movl	%eax, (%esp)
	movl	$5, 4(%esp)
	calll	listen
	.loc	1 113 36                # server.c:113:36
	cmpl	$0, %eax
.Ltmp12:
	.loc	1 113 6                 # server.c:113:6
	jge	.LBB0_8
# %bb.7:
.Ltmp13:
	.loc	1 114 3 is_stmt 1       # server.c:114:3
	leal	.L.str.5, %eax
	movl	%eax, (%esp)
	calll	do_error
.Ltmp14:
.LBB0_8:
	.loc	1 117 2                 # server.c:117:2
	jmp	.LBB0_9
.LBB0_9:                                # =>This Inner Loop Header: Depth=1
.Ltmp15:
	.loc	1 119 16                # server.c:119:16
	movl	$16, -52(%ebp)
	.loc	1 123 24                # server.c:123:24
	movl	-16(%ebp), %eax
	.loc	1 123 35 is_stmt 0      # server.c:123:35
	leal	-48(%ebp), %ecx
.Ltmp16:
	#DEBUG_VALUE: client_addr <- [$ecx+0]
	.loc	1 123 17                # server.c:123:17
	movl	%eax, (%esp)
	movl	%ecx, 4(%esp)
	leal	-52(%ebp), %eax
.Ltmp17:
	#DEBUG_VALUE: addr_len <- [$eax+0]
	movl	%eax, 8(%esp)
	calll	accept
.Ltmp18:
	.loc	1 123 15                # server.c:123:15
	movl	%eax, -72(%ebp)
.Ltmp19:
	.loc	1 124 20 is_stmt 1      # server.c:124:20
	cmpl	$0, -72(%ebp)
.Ltmp20:
	.loc	1 124 8 is_stmt 0       # server.c:124:8
	jge	.LBB0_11
# %bb.10:                               #   in Loop: Header=BB0_9 Depth=1
.Ltmp21:
	.loc	1 125 4 is_stmt 1       # server.c:125:4
	leal	.L.str.6, %eax
	movl	%eax, (%esp)
	calll	do_error
.Ltmp22:
.LBB0_11:                               #   in Loop: Header=BB0_9 Depth=1
	.loc	1 0 4 is_stmt 0         # server.c:0:4
	leal	-68(%ebp), %eax
	.loc	1 127 35 is_stmt 1      # server.c:127:35
	leal	-48(%ebp), %ecx
	addl	$4, %ecx
	.loc	1 127 3 is_stmt 0       # server.c:127:3
	movl	$2, (%esp)
	movl	%ecx, 4(%esp)
	movl	%eax, 8(%esp)
	movl	$16, 12(%esp)
	calll	inet_ntop
	leal	-68(%ebp), %ecx
	.loc	1 128 41 is_stmt 1      # server.c:128:41
	movzwl	-46(%ebp), %edx
	movl	%edx, (%esp)
	movl	%eax, -108(%ebp)        # 4-byte Spill
	movl	%ecx, -112(%ebp)        # 4-byte Spill
	calll	ntohs
	movzwl	%ax, %ecx
	.loc	1 128 3 is_stmt 0       # server.c:128:3
	leal	.L.str.7, %edx
	movl	%edx, (%esp)
	movl	-112(%ebp), %edx        # 4-byte Reload
	movl	%edx, 4(%esp)
	movl	%ecx, 8(%esp)
	calll	printf
	.loc	1 129 49 is_stmt 1      # server.c:129:49
	movl	-16(%ebp), %ecx
	.loc	1 129 59 is_stmt 0      # server.c:129:59
	movl	-72(%ebp), %edx
	.loc	1 129 3                 # server.c:129:3
	leal	.L.str.8, %esi
	movl	%esi, (%esp)
	movl	%ecx, 4(%esp)
	movl	%edx, 8(%esp)
	movl	%eax, -116(%ebp)        # 4-byte Spill
	calll	printf
	.loc	1 130 11 is_stmt 1      # server.c:130:11
	movl	-72(%ebp), %ecx
	.loc	1 130 3 is_stmt 0       # server.c:130:3
	movl	%ecx, (%esp)
	movl	%eax, -120(%ebp)        # 4-byte Spill
	calll	service
.Ltmp23:
	.loc	1 117 2 is_stmt 1       # server.c:117:2
	movl	%eax, -124(%ebp)        # 4-byte Spill
	jmp	.LBB0_9
.Ltmp24:
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.p2align	4, 0x90         # -- Begin function do_error
	.type	do_error,@function
do_error:                               # @do_error
.Lfunc_begin1:
	.loc	1 29 0                  # server.c:29:0
	.cfi_startproc
	popl	-16777216(%esp)
	subl	$4, %esp
# %bb.0:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset %ebp, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register %ebp
	subl	$8, %esp
	movl	8(%ebp), %eax
.Ltmp25:
	.loc	1 30 9 prologue_end     # server.c:30:9
	movl	8(%ebp), %ecx
	.loc	1 30 2 is_stmt 0        # server.c:30:2
	movl	%ecx, (%esp)
	movl	%eax, -4(%ebp)          # 4-byte Spill
	calll	perror
	.loc	1 31 2 is_stmt 1        # server.c:31:2
	movl	$1, (%esp)
	calll	exit
.Ltmp26:
.Lfunc_end1:
	.size	do_error, .Lfunc_end1-do_error
	.cfi_endproc
                                        # -- End function
	.p2align	4, 0x90         # -- Begin function service
	.type	service,@function
service:                                # @service
.Lfunc_begin2:
	.loc	1 58 0                  # server.c:58:0
	.cfi_startproc
	popl	-16777216(%esp)
	subl	$4, %esp
# %bb.0:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset %ebp, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register %ebp
	subl	$56, %esp
	movl	8(%ebp), %eax
.Ltmp27:
	.loc	1 62 13 prologue_end    # server.c:62:13
	movl	%eax, -12(%ebp)         # 4-byte Spill
	calll	fork
	.loc	1 62 11 is_stmt 0       # server.c:62:11
	movl	%eax, -8(%ebp)
	.loc	1 62 21                 # server.c:62:21
	cmpl	$0, %eax
.Ltmp28:
	.loc	1 62 6                  # server.c:62:6
	jge	.LBB2_2
# %bb.1:
.Ltmp29:
	.loc	1 63 3 is_stmt 1        # server.c:63:3
	leal	.L.str.9, %eax
	movl	%eax, (%esp)
	calll	perror
	.loc	1 64 2                  # server.c:64:2
	jmp	.LBB2_10
.Ltmp30:
.LBB2_2:
	.loc	1 64 15 is_stmt 0       # server.c:64:15
	cmpl	$0, -8(%ebp)
.Ltmp31:
	.loc	1 64 11                 # server.c:64:11
	jne	.LBB2_8
# %bb.3:
.Ltmp32:
	.loc	1 65 25 is_stmt 1       # server.c:65:25
	movl	8(%ebp), %eax
	.loc	1 65 3 is_stmt 0        # server.c:65:3
	leal	.L.str.10, %ecx
	movl	%ecx, (%esp)
	movl	%eax, 4(%esp)
	calll	printf
	xorl	%ecx, %ecx
.Ltmp33:
	.loc	1 66 11 is_stmt 1       # server.c:66:11
	movl	8(%ebp), %edx
	.loc	1 66 6 is_stmt 0        # server.c:66:6
	movl	%edx, (%esp)
	movl	$0, 4(%esp)
	movl	%eax, -16(%ebp)         # 4-byte Spill
	movl	%ecx, -20(%ebp)         # 4-byte Spill
	calll	dup2
	.loc	1 66 30                 # server.c:66:30
	cmpl	$0, %eax
	.loc	1 66 46                 # server.c:66:46
	jne	.LBB2_6
# %bb.4:
	.loc	1 67 10 is_stmt 1       # server.c:67:10
	movl	8(%ebp), %eax
	.loc	1 67 5 is_stmt 0        # server.c:67:5
	movl	%eax, (%esp)
	movl	$1, 4(%esp)
	calll	dup2
	.loc	1 67 30                 # server.c:67:30
	cmpl	$1, %eax
	.loc	1 67 47                 # server.c:67:47
	jne	.LBB2_6
# %bb.5:
	.loc	1 68 11 is_stmt 1       # server.c:68:11
	movl	8(%ebp), %eax
	.loc	1 68 6 is_stmt 0        # server.c:68:6
	movl	%eax, (%esp)
	movl	$2, 4(%esp)
	calll	dup2
	.loc	1 68 31                 # server.c:68:31
	cmpl	$2, %eax
.Ltmp34:
	.loc	1 66 6 is_stmt 1        # server.c:66:6
	je	.LBB2_7
.LBB2_6:
.Ltmp35:
	.loc	1 69 4                  # server.c:69:4
	leal	.L.str.11, %eax
	movl	%eax, (%esp)
	calll	do_error
.Ltmp36:
.LBB2_7:
	.loc	1 71 9                  # server.c:71:9
	movl	8(%ebp), %eax
	.loc	1 71 3 is_stmt 0        # server.c:71:3
	movl	%eax, (%esp)
	calll	close
	.loc	1 72 3 is_stmt 1        # server.c:72:3
	movl	%eax, -24(%ebp)         # 4-byte Spill
	calll	do_request
	xorl	%eax, %eax
	.loc	1 74 3                  # server.c:74:3
	movl	$0, (%esp)
	movl	%eax, -28(%ebp)         # 4-byte Spill
	calll	exit
.Ltmp37:
.LBB2_8:
	.loc	1 77 3                  # server.c:77:3
	movl	$-1, (%esp)
	leal	-4(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$1, 8(%esp)
	calll	waitpid
	.loc	1 78 9                  # server.c:78:9
	movl	8(%ebp), %ecx
	.loc	1 78 3 is_stmt 0        # server.c:78:3
	movl	%ecx, (%esp)
	movl	%eax, -32(%ebp)         # 4-byte Spill
	calll	close
	movl	%eax, -36(%ebp)         # 4-byte Spill
.Ltmp38:
# %bb.9:
	.loc	1 0 3                   # server.c:0:3
	jmp	.LBB2_10
.LBB2_10:
	xorl	%eax, %eax
	.loc	1 80 2 is_stmt 1        # server.c:80:2
	addl	$56, %esp
	popl	%ebp
	.cfi_def_cfa %esp, 4
	addl	$4, %esp
	pushl	-16777216(%esp)
	retl
.Ltmp39:
.Lfunc_end2:
	.size	service, .Lfunc_end2-service
	.cfi_endproc
                                        # -- End function
	.p2align	4, 0x90         # -- Begin function do_request
	.type	do_request,@function
do_request:                             # @do_request
.Lfunc_begin3:
	.loc	1 47 0                  # server.c:47:0
	.cfi_startproc
	popl	-16777216(%esp)
	subl	$4, %esp
# %bb.0:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset %ebp, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register %ebp
	subl	$8, %esp
.Ltmp40:
	.loc	1 48 2 prologue_end     # server.c:48:2
	jmp	.LBB3_1
.LBB3_1:                                # =>This Inner Loop Header: Depth=1
.Ltmp41:
	.loc	1 49 3                  # server.c:49:3
	calll	read_and_echo
.Ltmp42:
	.loc	1 50 6                  # server.c:50:6
	cmpl	$0, fptr
.Ltmp43:
	.loc	1 50 6 is_stmt 0        # server.c:50:6
	je	.LBB3_3
# %bb.2:                                #   in Loop: Header=BB3_1 Depth=1
.Ltmp44:
	.loc	1 53 4 is_stmt 1        # server.c:53:4
	leal	sbuf, %eax
	movl	%eax, (%esp)
	calll	*fptr
	movl	%eax, -4(%ebp)          # 4-byte Spill
.Ltmp45:
.LBB3_3:                                #   in Loop: Header=BB3_1 Depth=1
	.loc	1 48 2                  # server.c:48:2
	jmp	.LBB3_1
.Ltmp46:
.Lfunc_end3:
	.size	do_request, .Lfunc_end3-do_request
	.cfi_endproc
                                        # -- End function
	.p2align	4, 0x90         # -- Begin function read_and_echo
	.type	read_and_echo,@function
read_and_echo:                          # @read_and_echo
.Lfunc_begin4:
	.loc	1 38 0                  # server.c:38:0
	.cfi_startproc
	popl	-16777216(%esp)
	subl	$4, %esp
# %bb.0:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset %ebp, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register %ebp
	subl	$1048, %esp             # imm = 0x418
	leal	-1024(%ebp), %eax
.Ltmp47:
	#DEBUG_VALUE: read_and_echo:buffer <- [$eax+0]
	.loc	1 41 2 prologue_end     # server.c:41:2
	movl	%eax, (%esp)
	calll	gets
.Ltmp48:
	.loc	1 0 2 is_stmt 0         # server.c:0:2
	leal	-1024(%ebp), %ecx
	.loc	1 42 2 is_stmt 1        # server.c:42:2
	movl	%ecx, (%esp)
	movl	%eax, -1028(%ebp)       # 4-byte Spill
	calll	printf
	.loc	1 43 2                  # server.c:43:2
	leal	.L.str.12, %ecx
	movl	%ecx, (%esp)
	movl	%eax, -1032(%ebp)       # 4-byte Spill
	calll	printf
	.loc	1 44 1                  # server.c:44:1
	movl	%eax, -1036(%ebp)       # 4-byte Spill
	addl	$1048, %esp             # imm = 0x418
	popl	%ebp
	.cfi_def_cfa %esp, 4
	addl	$4, %esp
	pushl	-16777216(%esp)
	retl
.Ltmp49:
.Lfunc_end4:
	.size	read_and_echo, .Lfunc_end4-read_and_echo
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"Please enter:\t%s 8080 \n"
	.size	.L.str, 24

	.type	.L.str.1,@object        # @.str.1
.L.str.1:
	.asciz	"Server is running ...... \n"
	.size	.L.str.1, 27

	.type	.L.str.2,@object        # @.str.2
.L.str.2:
	.asciz	"%p %p\n"
	.size	.L.str.2, 7

	.type	sbuf,@object            # @sbuf
	.data
sbuf:
	.asciz	"0123456789012345678901234567890123456789"
	.size	sbuf, 41

	.type	fptr,@object            # @fptr
	.local	fptr
	.comm	fptr,4,4
	.type	.L.str.3,@object        # @.str.3
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.3:
	.asciz	"Socket() failed"
	.size	.L.str.3, 16

	.type	.L.str.4,@object        # @.str.4
.L.str.4:
	.asciz	"bind() failed"
	.size	.L.str.4, 14

	.type	.L.str.5,@object        # @.str.5
.L.str.5:
	.asciz	"listen() failed"
	.size	.L.str.5, 16

	.type	.L.str.6,@object        # @.str.6
.L.str.6:
	.asciz	"accept() failed"
	.size	.L.str.6, 16

	.type	.L.str.7,@object        # @.str.7
.L.str.7:
	.asciz	"Hello: %s/%d\n"
	.size	.L.str.7, 14

	.type	.L.str.8,@object        # @.str.8
.L.str.8:
	.asciz	"serv_sock = %d , client_sock = %d \n"
	.size	.L.str.8, 36

	.type	.L.str.9,@object        # @.str.9
.L.str.9:
	.asciz	"fork error"
	.size	.L.str.9, 11

	.type	.L.str.10,@object       # @.str.10
.L.str.10:
	.asciz	"clfd = %d \n"
	.size	.L.str.10, 12

	.type	.L.str.11,@object       # @.str.11
.L.str.11:
	.asciz	"dup2 error"
	.size	.L.str.11, 11

	.type	.L.str.12,@object       # @.str.12
.L.str.12:
	.asciz	"\n"
	.size	.L.str.12, 2

	.file	7 "/home/barra/src/R4/echo" "/usr/include/bits/types.h"
	.file	8 "/home/barra/src/R4/echo" "/usr/include/unistd.h"
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:
	.asciz	"clang version 7.0.0 (tags/RELEASE_700/final)" # string offset=0
.Linfo_string1:
	.asciz	"server.c"              # string offset=45
.Linfo_string2:
	.asciz	"/home/barra/src/R4/echo" # string offset=54
.Linfo_string3:
	.asciz	"MAXPENDING"            # string offset=78
.Linfo_string4:
	.asciz	"int"                   # string offset=89
.Linfo_string5:
	.asciz	"fptr"                  # string offset=93
.Linfo_string6:
	.asciz	"char"                  # string offset=98
.Linfo_string7:
	.asciz	"FUNPTR"                # string offset=103
.Linfo_string8:
	.asciz	"sbuf"                  # string offset=110
.Linfo_string9:
	.asciz	"__ARRAY_SIZE_TYPE__"   # string offset=115
.Linfo_string10:
	.asciz	"unsigned int"          # string offset=135
.Linfo_string11:
	.asciz	"SOCK_STREAM"           # string offset=148
.Linfo_string12:
	.asciz	"SOCK_DGRAM"            # string offset=160
.Linfo_string13:
	.asciz	"SOCK_RAW"              # string offset=171
.Linfo_string14:
	.asciz	"SOCK_RDM"              # string offset=180
.Linfo_string15:
	.asciz	"SOCK_SEQPACKET"        # string offset=189
.Linfo_string16:
	.asciz	"SOCK_DCCP"             # string offset=204
.Linfo_string17:
	.asciz	"SOCK_PACKET"           # string offset=214
.Linfo_string18:
	.asciz	"SOCK_CLOEXEC"          # string offset=226
.Linfo_string19:
	.asciz	"SOCK_NONBLOCK"         # string offset=239
.Linfo_string20:
	.asciz	"__socket_type"         # string offset=253
.Linfo_string21:
	.asciz	"IPPROTO_IP"            # string offset=267
.Linfo_string22:
	.asciz	"IPPROTO_ICMP"          # string offset=278
.Linfo_string23:
	.asciz	"IPPROTO_IGMP"          # string offset=291
.Linfo_string24:
	.asciz	"IPPROTO_IPIP"          # string offset=304
.Linfo_string25:
	.asciz	"IPPROTO_TCP"           # string offset=317
.Linfo_string26:
	.asciz	"IPPROTO_EGP"           # string offset=329
.Linfo_string27:
	.asciz	"IPPROTO_PUP"           # string offset=341
.Linfo_string28:
	.asciz	"IPPROTO_UDP"           # string offset=353
.Linfo_string29:
	.asciz	"IPPROTO_IDP"           # string offset=365
.Linfo_string30:
	.asciz	"IPPROTO_TP"            # string offset=377
.Linfo_string31:
	.asciz	"IPPROTO_DCCP"          # string offset=388
.Linfo_string32:
	.asciz	"IPPROTO_IPV6"          # string offset=401
.Linfo_string33:
	.asciz	"IPPROTO_RSVP"          # string offset=414
.Linfo_string34:
	.asciz	"IPPROTO_GRE"           # string offset=427
.Linfo_string35:
	.asciz	"IPPROTO_ESP"           # string offset=439
.Linfo_string36:
	.asciz	"IPPROTO_AH"            # string offset=451
.Linfo_string37:
	.asciz	"IPPROTO_MTP"           # string offset=462
.Linfo_string38:
	.asciz	"IPPROTO_BEETPH"        # string offset=474
.Linfo_string39:
	.asciz	"IPPROTO_ENCAP"         # string offset=489
.Linfo_string40:
	.asciz	"IPPROTO_PIM"           # string offset=503
.Linfo_string41:
	.asciz	"IPPROTO_COMP"          # string offset=515
.Linfo_string42:
	.asciz	"IPPROTO_SCTP"          # string offset=528
.Linfo_string43:
	.asciz	"IPPROTO_UDPLITE"       # string offset=541
.Linfo_string44:
	.asciz	"IPPROTO_MPLS"          # string offset=557
.Linfo_string45:
	.asciz	"IPPROTO_RAW"           # string offset=570
.Linfo_string46:
	.asciz	"IPPROTO_MAX"           # string offset=582
.Linfo_string47:
	.asciz	"uint32_t"              # string offset=594
.Linfo_string48:
	.asciz	"in_addr_t"             # string offset=603
.Linfo_string49:
	.asciz	"sa_family"             # string offset=613
.Linfo_string50:
	.asciz	"unsigned short"        # string offset=623
.Linfo_string51:
	.asciz	"sa_family_t"           # string offset=638
.Linfo_string52:
	.asciz	"sa_data"               # string offset=650
.Linfo_string53:
	.asciz	"sockaddr"              # string offset=658
.Linfo_string54:
	.asciz	"main"                  # string offset=667
.Linfo_string55:
	.asciz	"do_error"              # string offset=672
.Linfo_string56:
	.asciz	"service"               # string offset=681
.Linfo_string57:
	.asciz	"do_request"            # string offset=689
.Linfo_string58:
	.asciz	"read_and_echo"         # string offset=700
.Linfo_string59:
	.asciz	"argc"                  # string offset=714
.Linfo_string60:
	.asciz	"argv"                  # string offset=719
.Linfo_string61:
	.asciz	"port"                  # string offset=724
.Linfo_string62:
	.asciz	"serv_sock"             # string offset=729
.Linfo_string63:
	.asciz	"serv_addr"             # string offset=739
.Linfo_string64:
	.asciz	"sin_family"            # string offset=749
.Linfo_string65:
	.asciz	"sin_port"              # string offset=760
.Linfo_string66:
	.asciz	"uint16_t"              # string offset=769
.Linfo_string67:
	.asciz	"in_port_t"             # string offset=778
.Linfo_string68:
	.asciz	"sin_addr"              # string offset=788
.Linfo_string69:
	.asciz	"s_addr"                # string offset=797
.Linfo_string70:
	.asciz	"in_addr"               # string offset=804
.Linfo_string71:
	.asciz	"sin_zero"              # string offset=812
.Linfo_string72:
	.asciz	"unsigned char"         # string offset=821
.Linfo_string73:
	.asciz	"sockaddr_in"           # string offset=835
.Linfo_string74:
	.asciz	"client_addr"           # string offset=847
.Linfo_string75:
	.asciz	"addr_len"              # string offset=859
.Linfo_string76:
	.asciz	"client_name"           # string offset=868
.Linfo_string77:
	.asciz	"client_sock"           # string offset=880
.Linfo_string78:
	.asciz	"msg"                   # string offset=892
.Linfo_string79:
	.asciz	"clfd"                  # string offset=896
.Linfo_string80:
	.asciz	"status"                # string offset=901
.Linfo_string81:
	.asciz	"pid"                   # string offset=908
.Linfo_string82:
	.asciz	"__pid_t"               # string offset=912
.Linfo_string83:
	.asciz	"pid_t"                 # string offset=920
.Linfo_string84:
	.asciz	"buffer"                # string offset=926
	.section	.debug_abbrev,"",@progbits
	.byte	1                       # Abbreviation Code
	.byte	17                      # DW_TAG_compile_unit
	.byte	1                       # DW_CHILDREN_yes
	.byte	37                      # DW_AT_producer
	.byte	14                      # DW_FORM_strp
	.byte	19                      # DW_AT_language
	.byte	5                       # DW_FORM_data2
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	16                      # DW_AT_stmt_list
	.byte	23                      # DW_FORM_sec_offset
	.byte	27                      # DW_AT_comp_dir
	.byte	14                      # DW_FORM_strp
	.ascii	"\264B"                 # DW_AT_GNU_pubnames
	.byte	25                      # DW_FORM_flag_present
	.byte	17                      # DW_AT_low_pc
	.byte	1                       # DW_FORM_addr
	.byte	18                      # DW_AT_high_pc
	.byte	6                       # DW_FORM_data4
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	2                       # Abbreviation Code
	.byte	52                      # DW_TAG_variable
	.byte	0                       # DW_CHILDREN_no
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	28                      # DW_AT_const_value
	.byte	15                      # DW_FORM_udata
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	3                       # Abbreviation Code
	.byte	38                      # DW_TAG_const_type
	.byte	0                       # DW_CHILDREN_no
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	4                       # Abbreviation Code
	.byte	36                      # DW_TAG_base_type
	.byte	0                       # DW_CHILDREN_no
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	62                      # DW_AT_encoding
	.byte	11                      # DW_FORM_data1
	.byte	11                      # DW_AT_byte_size
	.byte	11                      # DW_FORM_data1
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	5                       # Abbreviation Code
	.byte	52                      # DW_TAG_variable
	.byte	0                       # DW_CHILDREN_no
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	2                       # DW_AT_location
	.byte	24                      # DW_FORM_exprloc
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	6                       # Abbreviation Code
	.byte	22                      # DW_TAG_typedef
	.byte	0                       # DW_CHILDREN_no
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	7                       # Abbreviation Code
	.byte	15                      # DW_TAG_pointer_type
	.byte	0                       # DW_CHILDREN_no
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	8                       # Abbreviation Code
	.byte	21                      # DW_TAG_subroutine_type
	.byte	1                       # DW_CHILDREN_yes
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	39                      # DW_AT_prototyped
	.byte	25                      # DW_FORM_flag_present
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	9                       # Abbreviation Code
	.byte	5                       # DW_TAG_formal_parameter
	.byte	0                       # DW_CHILDREN_no
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	10                      # Abbreviation Code
	.byte	1                       # DW_TAG_array_type
	.byte	1                       # DW_CHILDREN_yes
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	11                      # Abbreviation Code
	.byte	33                      # DW_TAG_subrange_type
	.byte	0                       # DW_CHILDREN_no
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	55                      # DW_AT_count
	.byte	11                      # DW_FORM_data1
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	12                      # Abbreviation Code
	.byte	36                      # DW_TAG_base_type
	.byte	0                       # DW_CHILDREN_no
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	11                      # DW_AT_byte_size
	.byte	11                      # DW_FORM_data1
	.byte	62                      # DW_AT_encoding
	.byte	11                      # DW_FORM_data1
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	13                      # Abbreviation Code
	.byte	4                       # DW_TAG_enumeration_type
	.byte	1                       # DW_CHILDREN_yes
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	11                      # DW_AT_byte_size
	.byte	11                      # DW_FORM_data1
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	14                      # Abbreviation Code
	.byte	40                      # DW_TAG_enumerator
	.byte	0                       # DW_CHILDREN_no
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	28                      # DW_AT_const_value
	.byte	15                      # DW_FORM_udata
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	15                      # Abbreviation Code
	.byte	4                       # DW_TAG_enumeration_type
	.byte	1                       # DW_CHILDREN_yes
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	11                      # DW_AT_byte_size
	.byte	11                      # DW_FORM_data1
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	16                      # Abbreviation Code
	.byte	19                      # DW_TAG_structure_type
	.byte	1                       # DW_CHILDREN_yes
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	11                      # DW_AT_byte_size
	.byte	11                      # DW_FORM_data1
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	17                      # Abbreviation Code
	.byte	13                      # DW_TAG_member
	.byte	0                       # DW_CHILDREN_no
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	56                      # DW_AT_data_member_location
	.byte	11                      # DW_FORM_data1
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	18                      # Abbreviation Code
	.byte	46                      # DW_TAG_subprogram
	.byte	1                       # DW_CHILDREN_yes
	.byte	17                      # DW_AT_low_pc
	.byte	1                       # DW_FORM_addr
	.byte	18                      # DW_AT_high_pc
	.byte	6                       # DW_FORM_data4
	.byte	64                      # DW_AT_frame_base
	.byte	24                      # DW_FORM_exprloc
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	39                      # DW_AT_prototyped
	.byte	25                      # DW_FORM_flag_present
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	63                      # DW_AT_external
	.byte	25                      # DW_FORM_flag_present
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	19                      # Abbreviation Code
	.byte	5                       # DW_TAG_formal_parameter
	.byte	0                       # DW_CHILDREN_no
	.byte	2                       # DW_AT_location
	.byte	24                      # DW_FORM_exprloc
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	20                      # Abbreviation Code
	.byte	52                      # DW_TAG_variable
	.byte	0                       # DW_CHILDREN_no
	.byte	2                       # DW_AT_location
	.byte	24                      # DW_FORM_exprloc
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	21                      # Abbreviation Code
	.byte	11                      # DW_TAG_lexical_block
	.byte	1                       # DW_CHILDREN_yes
	.byte	17                      # DW_AT_low_pc
	.byte	1                       # DW_FORM_addr
	.byte	18                      # DW_AT_high_pc
	.byte	6                       # DW_FORM_data4
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	22                      # Abbreviation Code
	.byte	46                      # DW_TAG_subprogram
	.byte	1                       # DW_CHILDREN_yes
	.byte	17                      # DW_AT_low_pc
	.byte	1                       # DW_FORM_addr
	.byte	18                      # DW_AT_high_pc
	.byte	6                       # DW_FORM_data4
	.byte	64                      # DW_AT_frame_base
	.byte	24                      # DW_FORM_exprloc
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	39                      # DW_AT_prototyped
	.byte	25                      # DW_FORM_flag_present
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	23                      # Abbreviation Code
	.byte	24                      # DW_TAG_unspecified_parameters
	.byte	0                       # DW_CHILDREN_no
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	24                      # Abbreviation Code
	.byte	46                      # DW_TAG_subprogram
	.byte	1                       # DW_CHILDREN_yes
	.byte	17                      # DW_AT_low_pc
	.byte	1                       # DW_FORM_addr
	.byte	18                      # DW_AT_high_pc
	.byte	6                       # DW_FORM_data4
	.byte	64                      # DW_AT_frame_base
	.byte	24                      # DW_FORM_exprloc
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	39                      # DW_AT_prototyped
	.byte	25                      # DW_FORM_flag_present
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	25                      # Abbreviation Code
	.byte	46                      # DW_TAG_subprogram
	.byte	0                       # DW_CHILDREN_no
	.byte	17                      # DW_AT_low_pc
	.byte	1                       # DW_FORM_addr
	.byte	18                      # DW_AT_high_pc
	.byte	6                       # DW_FORM_data4
	.byte	64                      # DW_AT_frame_base
	.byte	24                      # DW_FORM_exprloc
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	11                      # DW_FORM_data1
	.byte	39                      # DW_AT_prototyped
	.byte	25                      # DW_FORM_flag_present
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	26                      # Abbreviation Code
	.byte	22                      # DW_TAG_typedef
	.byte	0                       # DW_CHILDREN_no
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	3                       # DW_AT_name
	.byte	14                      # DW_FORM_strp
	.byte	58                      # DW_AT_decl_file
	.byte	11                      # DW_FORM_data1
	.byte	59                      # DW_AT_decl_line
	.byte	5                       # DW_FORM_data2
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	27                      # Abbreviation Code
	.byte	33                      # DW_TAG_subrange_type
	.byte	0                       # DW_CHILDREN_no
	.byte	73                      # DW_AT_type
	.byte	19                      # DW_FORM_ref4
	.byte	55                      # DW_AT_count
	.byte	5                       # DW_FORM_data2
	.byte	0                       # EOM(1)
	.byte	0                       # EOM(2)
	.byte	0                       # EOM(3)
	.section	.debug_info,"",@progbits
.Lcu_begin0:
	.long	977                     # Length of Unit
	.short	4                       # DWARF version number
	.long	.debug_abbrev           # Offset Into Abbrev. Section
	.byte	4                       # Address Size (in bytes)
	.byte	1                       # Abbrev [1] 0xb:0x3ca DW_TAG_compile_unit
	.long	.Linfo_string0          # DW_AT_producer
	.short	12                      # DW_AT_language
	.long	.Linfo_string1          # DW_AT_name
	.long	.Lline_table_start0     # DW_AT_stmt_list
	.long	.Linfo_string2          # DW_AT_comp_dir
                                        # DW_AT_GNU_pubnames
	.long	.Lfunc_begin0           # DW_AT_low_pc
	.long	.Lfunc_end4-.Lfunc_begin0 # DW_AT_high_pc
	.byte	2                       # Abbrev [2] 0x26:0xc DW_TAG_variable
	.long	.Linfo_string3          # DW_AT_name
	.long	50                      # DW_AT_type
	.byte	1                       # DW_AT_decl_file
	.byte	22                      # DW_AT_decl_line
	.byte	5                       # DW_AT_const_value
	.byte	3                       # Abbrev [3] 0x32:0x5 DW_TAG_const_type
	.long	55                      # DW_AT_type
	.byte	4                       # Abbrev [4] 0x37:0x7 DW_TAG_base_type
	.long	.Linfo_string4          # DW_AT_name
	.byte	5                       # DW_AT_encoding
	.byte	4                       # DW_AT_byte_size
	.byte	5                       # Abbrev [5] 0x3e:0x11 DW_TAG_variable
	.long	.Linfo_string5          # DW_AT_name
	.long	79                      # DW_AT_type
	.byte	1                       # DW_AT_decl_file
	.byte	25                      # DW_AT_decl_line
	.byte	5                       # DW_AT_location
	.byte	3
	.long	fptr
	.byte	6                       # Abbrev [6] 0x4f:0xb DW_TAG_typedef
	.long	90                      # DW_AT_type
	.long	.Linfo_string7          # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	23                      # DW_AT_decl_line
	.byte	7                       # Abbrev [7] 0x5a:0x5 DW_TAG_pointer_type
	.long	95                      # DW_AT_type
	.byte	8                       # Abbrev [8] 0x5f:0xb DW_TAG_subroutine_type
	.long	55                      # DW_AT_type
                                        # DW_AT_prototyped
	.byte	9                       # Abbrev [9] 0x64:0x5 DW_TAG_formal_parameter
	.long	106                     # DW_AT_type
	.byte	0                       # End Of Children Mark
	.byte	7                       # Abbrev [7] 0x6a:0x5 DW_TAG_pointer_type
	.long	111                     # DW_AT_type
	.byte	3                       # Abbrev [3] 0x6f:0x5 DW_TAG_const_type
	.long	116                     # DW_AT_type
	.byte	4                       # Abbrev [4] 0x74:0x7 DW_TAG_base_type
	.long	.Linfo_string6          # DW_AT_name
	.byte	6                       # DW_AT_encoding
	.byte	1                       # DW_AT_byte_size
	.byte	5                       # Abbrev [5] 0x7b:0x11 DW_TAG_variable
	.long	.Linfo_string8          # DW_AT_name
	.long	140                     # DW_AT_type
	.byte	1                       # DW_AT_decl_file
	.byte	24                      # DW_AT_decl_line
	.byte	5                       # DW_AT_location
	.byte	3
	.long	sbuf
	.byte	10                      # Abbrev [10] 0x8c:0xc DW_TAG_array_type
	.long	116                     # DW_AT_type
	.byte	11                      # Abbrev [11] 0x91:0x6 DW_TAG_subrange_type
	.long	152                     # DW_AT_type
	.byte	41                      # DW_AT_count
	.byte	0                       # End Of Children Mark
	.byte	12                      # Abbrev [12] 0x98:0x7 DW_TAG_base_type
	.long	.Linfo_string9          # DW_AT_name
	.byte	8                       # DW_AT_byte_size
	.byte	7                       # DW_AT_encoding
	.byte	13                      # Abbrev [13] 0x9f:0x46 DW_TAG_enumeration_type
	.long	229                     # DW_AT_type
	.long	.Linfo_string20         # DW_AT_name
	.byte	4                       # DW_AT_byte_size
	.byte	2                       # DW_AT_decl_file
	.byte	24                      # DW_AT_decl_line
	.byte	14                      # Abbrev [14] 0xab:0x6 DW_TAG_enumerator
	.long	.Linfo_string11         # DW_AT_name
	.byte	1                       # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0xb1:0x6 DW_TAG_enumerator
	.long	.Linfo_string12         # DW_AT_name
	.byte	2                       # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0xb7:0x6 DW_TAG_enumerator
	.long	.Linfo_string13         # DW_AT_name
	.byte	3                       # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0xbd:0x6 DW_TAG_enumerator
	.long	.Linfo_string14         # DW_AT_name
	.byte	4                       # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0xc3:0x6 DW_TAG_enumerator
	.long	.Linfo_string15         # DW_AT_name
	.byte	5                       # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0xc9:0x6 DW_TAG_enumerator
	.long	.Linfo_string16         # DW_AT_name
	.byte	6                       # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0xcf:0x6 DW_TAG_enumerator
	.long	.Linfo_string17         # DW_AT_name
	.byte	10                      # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0xd5:0x8 DW_TAG_enumerator
	.long	.Linfo_string18         # DW_AT_name
	.ascii	"\200\200 "             # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0xdd:0x7 DW_TAG_enumerator
	.long	.Linfo_string19         # DW_AT_name
	.ascii	"\200\020"              # DW_AT_const_value
	.byte	0                       # End Of Children Mark
	.byte	4                       # Abbrev [4] 0xe5:0x7 DW_TAG_base_type
	.long	.Linfo_string10         # DW_AT_name
	.byte	7                       # DW_AT_encoding
	.byte	4                       # DW_AT_byte_size
	.byte	15                      # Abbrev [15] 0xec:0xaa DW_TAG_enumeration_type
	.long	229                     # DW_AT_type
	.byte	4                       # DW_AT_byte_size
	.byte	3                       # DW_AT_decl_file
	.byte	40                      # DW_AT_decl_line
	.byte	14                      # Abbrev [14] 0xf4:0x6 DW_TAG_enumerator
	.long	.Linfo_string21         # DW_AT_name
	.byte	0                       # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0xfa:0x6 DW_TAG_enumerator
	.long	.Linfo_string22         # DW_AT_name
	.byte	1                       # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x100:0x6 DW_TAG_enumerator
	.long	.Linfo_string23         # DW_AT_name
	.byte	2                       # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x106:0x6 DW_TAG_enumerator
	.long	.Linfo_string24         # DW_AT_name
	.byte	4                       # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x10c:0x6 DW_TAG_enumerator
	.long	.Linfo_string25         # DW_AT_name
	.byte	6                       # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x112:0x6 DW_TAG_enumerator
	.long	.Linfo_string26         # DW_AT_name
	.byte	8                       # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x118:0x6 DW_TAG_enumerator
	.long	.Linfo_string27         # DW_AT_name
	.byte	12                      # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x11e:0x6 DW_TAG_enumerator
	.long	.Linfo_string28         # DW_AT_name
	.byte	17                      # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x124:0x6 DW_TAG_enumerator
	.long	.Linfo_string29         # DW_AT_name
	.byte	22                      # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x12a:0x6 DW_TAG_enumerator
	.long	.Linfo_string30         # DW_AT_name
	.byte	29                      # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x130:0x6 DW_TAG_enumerator
	.long	.Linfo_string31         # DW_AT_name
	.byte	33                      # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x136:0x6 DW_TAG_enumerator
	.long	.Linfo_string32         # DW_AT_name
	.byte	41                      # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x13c:0x6 DW_TAG_enumerator
	.long	.Linfo_string33         # DW_AT_name
	.byte	46                      # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x142:0x6 DW_TAG_enumerator
	.long	.Linfo_string34         # DW_AT_name
	.byte	47                      # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x148:0x6 DW_TAG_enumerator
	.long	.Linfo_string35         # DW_AT_name
	.byte	50                      # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x14e:0x6 DW_TAG_enumerator
	.long	.Linfo_string36         # DW_AT_name
	.byte	51                      # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x154:0x6 DW_TAG_enumerator
	.long	.Linfo_string37         # DW_AT_name
	.byte	92                      # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x15a:0x6 DW_TAG_enumerator
	.long	.Linfo_string38         # DW_AT_name
	.byte	94                      # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x160:0x6 DW_TAG_enumerator
	.long	.Linfo_string39         # DW_AT_name
	.byte	98                      # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x166:0x6 DW_TAG_enumerator
	.long	.Linfo_string40         # DW_AT_name
	.byte	103                     # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x16c:0x6 DW_TAG_enumerator
	.long	.Linfo_string41         # DW_AT_name
	.byte	108                     # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x172:0x7 DW_TAG_enumerator
	.long	.Linfo_string42         # DW_AT_name
	.ascii	"\204\001"              # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x179:0x7 DW_TAG_enumerator
	.long	.Linfo_string43         # DW_AT_name
	.ascii	"\210\001"              # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x180:0x7 DW_TAG_enumerator
	.long	.Linfo_string44         # DW_AT_name
	.ascii	"\211\001"              # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x187:0x7 DW_TAG_enumerator
	.long	.Linfo_string45         # DW_AT_name
	.ascii	"\377\001"              # DW_AT_const_value
	.byte	14                      # Abbrev [14] 0x18e:0x7 DW_TAG_enumerator
	.long	.Linfo_string46         # DW_AT_name
	.ascii	"\200\002"              # DW_AT_const_value
	.byte	0                       # End Of Children Mark
	.byte	6                       # Abbrev [6] 0x196:0xb DW_TAG_typedef
	.long	417                     # DW_AT_type
	.long	.Linfo_string48         # DW_AT_name
	.byte	3                       # DW_AT_decl_file
	.byte	30                      # DW_AT_decl_line
	.byte	6                       # Abbrev [6] 0x1a1:0xb DW_TAG_typedef
	.long	229                     # DW_AT_type
	.long	.Linfo_string47         # DW_AT_name
	.byte	4                       # DW_AT_decl_file
	.byte	51                      # DW_AT_decl_line
	.byte	7                       # Abbrev [7] 0x1ac:0x5 DW_TAG_pointer_type
	.long	433                     # DW_AT_type
	.byte	16                      # Abbrev [16] 0x1b1:0x21 DW_TAG_structure_type
	.long	.Linfo_string53         # DW_AT_name
	.byte	16                      # DW_AT_byte_size
	.byte	6                       # DW_AT_decl_file
	.byte	153                     # DW_AT_decl_line
	.byte	17                      # Abbrev [17] 0x1b9:0xc DW_TAG_member
	.long	.Linfo_string49         # DW_AT_name
	.long	466                     # DW_AT_type
	.byte	6                       # DW_AT_decl_file
	.byte	155                     # DW_AT_decl_line
	.byte	0                       # DW_AT_data_member_location
	.byte	17                      # Abbrev [17] 0x1c5:0xc DW_TAG_member
	.long	.Linfo_string52         # DW_AT_name
	.long	484                     # DW_AT_type
	.byte	6                       # DW_AT_decl_file
	.byte	156                     # DW_AT_decl_line
	.byte	2                       # DW_AT_data_member_location
	.byte	0                       # End Of Children Mark
	.byte	6                       # Abbrev [6] 0x1d2:0xb DW_TAG_typedef
	.long	477                     # DW_AT_type
	.long	.Linfo_string51         # DW_AT_name
	.byte	5                       # DW_AT_decl_file
	.byte	28                      # DW_AT_decl_line
	.byte	4                       # Abbrev [4] 0x1dd:0x7 DW_TAG_base_type
	.long	.Linfo_string50         # DW_AT_name
	.byte	7                       # DW_AT_encoding
	.byte	2                       # DW_AT_byte_size
	.byte	10                      # Abbrev [10] 0x1e4:0xc DW_TAG_array_type
	.long	116                     # DW_AT_type
	.byte	11                      # Abbrev [11] 0x1e9:0x6 DW_TAG_subrange_type
	.long	152                     # DW_AT_type
	.byte	14                      # DW_AT_count
	.byte	0                       # End Of Children Mark
	.byte	18                      # Abbrev [18] 0x1f0:0xa0 DW_TAG_subprogram
	.long	.Lfunc_begin0           # DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0 # DW_AT_high_pc
	.byte	1                       # DW_AT_frame_base
	.byte	85
	.long	.Linfo_string54         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	83                      # DW_AT_decl_line
                                        # DW_AT_prototyped
	.long	55                      # DW_AT_type
                                        # DW_AT_external
	.byte	19                      # Abbrev [19] 0x205:0xe DW_TAG_formal_parameter
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	8
	.long	.Linfo_string59         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	83                      # DW_AT_decl_line
	.long	55                      # DW_AT_type
	.byte	19                      # Abbrev [19] 0x213:0xe DW_TAG_formal_parameter
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	12
	.long	.Linfo_string60         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	83                      # DW_AT_decl_line
	.long	803                     # DW_AT_type
	.byte	20                      # Abbrev [20] 0x221:0xe DW_TAG_variable
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	116
	.long	.Linfo_string61         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	84                      # DW_AT_decl_line
	.long	55                      # DW_AT_type
	.byte	20                      # Abbrev [20] 0x22f:0xe DW_TAG_variable
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	112
	.long	.Linfo_string62         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	85                      # DW_AT_decl_line
	.long	55                      # DW_AT_type
	.byte	20                      # Abbrev [20] 0x23d:0xe DW_TAG_variable
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	96
	.long	.Linfo_string63         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	86                      # DW_AT_decl_line
	.long	813                     # DW_AT_type
	.byte	21                      # Abbrev [21] 0x24b:0x44 DW_TAG_lexical_block
	.long	.Ltmp15                 # DW_AT_low_pc
	.long	.Ltmp23-.Ltmp15         # DW_AT_high_pc
	.byte	20                      # Abbrev [20] 0x254:0xe DW_TAG_variable
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	80
	.long	.Linfo_string74         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	118                     # DW_AT_decl_line
	.long	813                     # DW_AT_type
	.byte	20                      # Abbrev [20] 0x262:0xe DW_TAG_variable
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	76
	.long	.Linfo_string75         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	119                     # DW_AT_decl_line
	.long	229                     # DW_AT_type
	.byte	20                      # Abbrev [20] 0x270:0xf DW_TAG_variable
	.byte	3                       # DW_AT_location
	.byte	145
	.ascii	"\274\177"
	.long	.Linfo_string76         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	120                     # DW_AT_decl_line
	.long	932                     # DW_AT_type
	.byte	20                      # Abbrev [20] 0x27f:0xf DW_TAG_variable
	.byte	3                       # DW_AT_location
	.byte	145
	.ascii	"\270\177"
	.long	.Linfo_string77         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	121                     # DW_AT_decl_line
	.long	55                      # DW_AT_type
	.byte	0                       # End Of Children Mark
	.byte	0                       # End Of Children Mark
	.byte	22                      # Abbrev [22] 0x290:0x21 DW_TAG_subprogram
	.long	.Lfunc_begin1           # DW_AT_low_pc
	.long	.Lfunc_end1-.Lfunc_begin1 # DW_AT_high_pc
	.byte	1                       # DW_AT_frame_base
	.byte	85
	.long	.Linfo_string55         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	29                      # DW_AT_decl_line
                                        # DW_AT_prototyped
	.byte	19                      # Abbrev [19] 0x2a1:0xe DW_TAG_formal_parameter
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	8
	.long	.Linfo_string78         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	29                      # DW_AT_decl_line
	.long	106                     # DW_AT_type
	.byte	23                      # Abbrev [23] 0x2af:0x1 DW_TAG_unspecified_parameters
	.byte	0                       # End Of Children Mark
	.byte	24                      # Abbrev [24] 0x2b1:0x40 DW_TAG_subprogram
	.long	.Lfunc_begin2           # DW_AT_low_pc
	.long	.Lfunc_end2-.Lfunc_begin2 # DW_AT_high_pc
	.byte	1                       # DW_AT_frame_base
	.byte	85
	.long	.Linfo_string56         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	58                      # DW_AT_decl_line
                                        # DW_AT_prototyped
	.long	55                      # DW_AT_type
	.byte	19                      # Abbrev [19] 0x2c6:0xe DW_TAG_formal_parameter
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	8
	.long	.Linfo_string79         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	58                      # DW_AT_decl_line
	.long	55                      # DW_AT_type
	.byte	20                      # Abbrev [20] 0x2d4:0xe DW_TAG_variable
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	124
	.long	.Linfo_string80         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	59                      # DW_AT_decl_line
	.long	55                      # DW_AT_type
	.byte	20                      # Abbrev [20] 0x2e2:0xe DW_TAG_variable
	.byte	2                       # DW_AT_location
	.byte	145
	.byte	120
	.long	.Linfo_string81         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	60                      # DW_AT_decl_line
	.long	944                     # DW_AT_type
	.byte	0                       # End Of Children Mark
	.byte	25                      # Abbrev [25] 0x2f1:0x11 DW_TAG_subprogram
	.long	.Lfunc_begin3           # DW_AT_low_pc
	.long	.Lfunc_end3-.Lfunc_begin3 # DW_AT_high_pc
	.byte	1                       # DW_AT_frame_base
	.byte	85
	.long	.Linfo_string57         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	47                      # DW_AT_decl_line
                                        # DW_AT_prototyped
	.byte	22                      # Abbrev [22] 0x302:0x21 DW_TAG_subprogram
	.long	.Lfunc_begin4           # DW_AT_low_pc
	.long	.Lfunc_end4-.Lfunc_begin4 # DW_AT_high_pc
	.byte	1                       # DW_AT_frame_base
	.byte	85
	.long	.Linfo_string58         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	38                      # DW_AT_decl_line
                                        # DW_AT_prototyped
	.byte	20                      # Abbrev [20] 0x313:0xf DW_TAG_variable
	.byte	3                       # DW_AT_location
	.byte	145
	.ascii	"\200x"
	.long	.Linfo_string84         # DW_AT_name
	.byte	1                       # DW_AT_decl_file
	.byte	39                      # DW_AT_decl_line
	.long	967                     # DW_AT_type
	.byte	0                       # End Of Children Mark
	.byte	7                       # Abbrev [7] 0x323:0x5 DW_TAG_pointer_type
	.long	808                     # DW_AT_type
	.byte	7                       # Abbrev [7] 0x328:0x5 DW_TAG_pointer_type
	.long	116                     # DW_AT_type
	.byte	16                      # Abbrev [16] 0x32d:0x39 DW_TAG_structure_type
	.long	.Linfo_string73         # DW_AT_name
	.byte	16                      # DW_AT_byte_size
	.byte	3                       # DW_AT_decl_file
	.byte	239                     # DW_AT_decl_line
	.byte	17                      # Abbrev [17] 0x335:0xc DW_TAG_member
	.long	.Linfo_string64         # DW_AT_name
	.long	466                     # DW_AT_type
	.byte	3                       # DW_AT_decl_file
	.byte	241                     # DW_AT_decl_line
	.byte	0                       # DW_AT_data_member_location
	.byte	17                      # Abbrev [17] 0x341:0xc DW_TAG_member
	.long	.Linfo_string65         # DW_AT_name
	.long	870                     # DW_AT_type
	.byte	3                       # DW_AT_decl_file
	.byte	242                     # DW_AT_decl_line
	.byte	2                       # DW_AT_data_member_location
	.byte	17                      # Abbrev [17] 0x34d:0xc DW_TAG_member
	.long	.Linfo_string68         # DW_AT_name
	.long	892                     # DW_AT_type
	.byte	3                       # DW_AT_decl_file
	.byte	243                     # DW_AT_decl_line
	.byte	4                       # DW_AT_data_member_location
	.byte	17                      # Abbrev [17] 0x359:0xc DW_TAG_member
	.long	.Linfo_string71         # DW_AT_name
	.long	913                     # DW_AT_type
	.byte	3                       # DW_AT_decl_file
	.byte	246                     # DW_AT_decl_line
	.byte	8                       # DW_AT_data_member_location
	.byte	0                       # End Of Children Mark
	.byte	6                       # Abbrev [6] 0x366:0xb DW_TAG_typedef
	.long	881                     # DW_AT_type
	.long	.Linfo_string67         # DW_AT_name
	.byte	3                       # DW_AT_decl_file
	.byte	119                     # DW_AT_decl_line
	.byte	6                       # Abbrev [6] 0x371:0xb DW_TAG_typedef
	.long	477                     # DW_AT_type
	.long	.Linfo_string66         # DW_AT_name
	.byte	4                       # DW_AT_decl_file
	.byte	49                      # DW_AT_decl_line
	.byte	16                      # Abbrev [16] 0x37c:0x15 DW_TAG_structure_type
	.long	.Linfo_string70         # DW_AT_name
	.byte	4                       # DW_AT_byte_size
	.byte	3                       # DW_AT_decl_file
	.byte	31                      # DW_AT_decl_line
	.byte	17                      # Abbrev [17] 0x384:0xc DW_TAG_member
	.long	.Linfo_string69         # DW_AT_name
	.long	406                     # DW_AT_type
	.byte	3                       # DW_AT_decl_file
	.byte	33                      # DW_AT_decl_line
	.byte	0                       # DW_AT_data_member_location
	.byte	0                       # End Of Children Mark
	.byte	10                      # Abbrev [10] 0x391:0xc DW_TAG_array_type
	.long	925                     # DW_AT_type
	.byte	11                      # Abbrev [11] 0x396:0x6 DW_TAG_subrange_type
	.long	152                     # DW_AT_type
	.byte	8                       # DW_AT_count
	.byte	0                       # End Of Children Mark
	.byte	4                       # Abbrev [4] 0x39d:0x7 DW_TAG_base_type
	.long	.Linfo_string72         # DW_AT_name
	.byte	8                       # DW_AT_encoding
	.byte	1                       # DW_AT_byte_size
	.byte	10                      # Abbrev [10] 0x3a4:0xc DW_TAG_array_type
	.long	116                     # DW_AT_type
	.byte	11                      # Abbrev [11] 0x3a9:0x6 DW_TAG_subrange_type
	.long	152                     # DW_AT_type
	.byte	16                      # DW_AT_count
	.byte	0                       # End Of Children Mark
	.byte	26                      # Abbrev [26] 0x3b0:0xc DW_TAG_typedef
	.long	956                     # DW_AT_type
	.long	.Linfo_string83         # DW_AT_name
	.byte	8                       # DW_AT_decl_file
	.short	263                     # DW_AT_decl_line
	.byte	6                       # Abbrev [6] 0x3bc:0xb DW_TAG_typedef
	.long	55                      # DW_AT_type
	.long	.Linfo_string82         # DW_AT_name
	.byte	7                       # DW_AT_decl_file
	.byte	133                     # DW_AT_decl_line
	.byte	10                      # Abbrev [10] 0x3c7:0xd DW_TAG_array_type
	.long	116                     # DW_AT_type
	.byte	27                      # Abbrev [27] 0x3cc:0x7 DW_TAG_subrange_type
	.long	152                     # DW_AT_type
	.short	1024                    # DW_AT_count
	.byte	0                       # End Of Children Mark
	.byte	0                       # End Of Children Mark
	.section	.debug_macinfo,"",@progbits
	.byte	0                       # End Of Macro List Mark
	.section	.debug_pubnames,"",@progbits
	.long	.LpubNames_end0-.LpubNames_begin0 # Length of Public Names Info
.LpubNames_begin0:
	.short	2                       # DWARF Version
	.long	.Lcu_begin0             # Offset of Compilation Unit Info
	.long	981                     # Compilation Unit Length
	.long	123                     # DIE offset
	.asciz	"sbuf"                  # External Name
	.long	689                     # DIE offset
	.asciz	"service"               # External Name
	.long	496                     # DIE offset
	.asciz	"main"                  # External Name
	.long	38                      # DIE offset
	.asciz	"MAXPENDING"            # External Name
	.long	62                      # DIE offset
	.asciz	"fptr"                  # External Name
	.long	656                     # DIE offset
	.asciz	"do_error"              # External Name
	.long	753                     # DIE offset
	.asciz	"do_request"            # External Name
	.long	770                     # DIE offset
	.asciz	"read_and_echo"         # External Name
	.long	0                       # End Mark
.LpubNames_end0:
	.section	.debug_pubtypes,"",@progbits
	.long	.LpubTypes_end0-.LpubTypes_begin0 # Length of Public Types Info
.LpubTypes_begin0:
	.short	2                       # DWARF Version
	.long	.Lcu_begin0             # Offset of Compilation Unit Info
	.long	981                     # Compilation Unit Length
	.long	813                     # DIE offset
	.asciz	"sockaddr_in"           # External Name
	.long	406                     # DIE offset
	.asciz	"in_addr_t"             # External Name
	.long	466                     # DIE offset
	.asciz	"sa_family_t"           # External Name
	.long	159                     # DIE offset
	.asciz	"__socket_type"         # External Name
	.long	229                     # DIE offset
	.asciz	"unsigned int"          # External Name
	.long	55                      # DIE offset
	.asciz	"int"                   # External Name
	.long	477                     # DIE offset
	.asciz	"unsigned short"        # External Name
	.long	870                     # DIE offset
	.asciz	"in_port_t"             # External Name
	.long	956                     # DIE offset
	.asciz	"__pid_t"               # External Name
	.long	944                     # DIE offset
	.asciz	"pid_t"                 # External Name
	.long	433                     # DIE offset
	.asciz	"sockaddr"              # External Name
	.long	892                     # DIE offset
	.asciz	"in_addr"               # External Name
	.long	417                     # DIE offset
	.asciz	"uint32_t"              # External Name
	.long	881                     # DIE offset
	.asciz	"uint16_t"              # External Name
	.long	925                     # DIE offset
	.asciz	"unsigned char"         # External Name
	.long	116                     # DIE offset
	.asciz	"char"                  # External Name
	.long	79                      # DIE offset
	.asciz	"FUNPTR"                # External Name
	.long	0                       # End Mark
.LpubTypes_end0:

	.ident	"clang version 7.0.0 (tags/RELEASE_700/final)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym main
	.addrsig_sym printf
	.addrsig_sym atoi
	.addrsig_sym socket
	.addrsig_sym do_error
	.addrsig_sym memset
	.addrsig_sym htonl
	.addrsig_sym htons
	.addrsig_sym bind
	.addrsig_sym listen
	.addrsig_sym accept
	.addrsig_sym inet_ntop
	.addrsig_sym ntohs
	.addrsig_sym service
	.addrsig_sym perror
	.addrsig_sym exit
	.addrsig_sym fork
	.addrsig_sym dup2
	.addrsig_sym close
	.addrsig_sym do_request
	.addrsig_sym waitpid
	.addrsig_sym read_and_echo
	.addrsig_sym gets
	.addrsig_sym sbuf
	.addrsig_sym fptr
	.section	.debug_line,"",@progbits
.Lline_table_start0:
