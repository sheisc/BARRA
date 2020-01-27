
/***********************************************************************
			A vulnerable multi-process Echo server
	(1) format string vulnerability and buffer overflow in read_and_echo().
	(2) A toy server for fun.
	
	sheisc@163.com
 ***********************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>


#define BUFSIZE  1024

static const int MAXPENDING = 5;
typedef int (* FUNPTR)(const char *command);
static char sbuf[] = "0123456789012345678901234567890123456789";
static FUNPTR fptr;



static void do_error(const char *msg, ...) {
	perror(msg);
	exit(1);
}
/**
	Vulnerable Function
	(1) Buffer overflow caused by gets()
	(2) Format string vulnerability deriving from incorrect use of printf()
 */
static void read_and_echo(void){
	char buffer[BUFSIZE];	
	
	gets(buffer);
	printf(buffer);
	printf("\n");	
}

//
static void do_request(void){
	while(1){
		read_and_echo();		
		if(fptr){
			// Due to the vulnerability in read_and_echo(),
			// this call can be exploited to invoke system("/bin/sh");
			fptr(sbuf);
		}
	}
}

static int service(int clfd){
	int status;	
	pid_t pid;

	if( (pid = fork()) < 0){
		perror("fork error");
	}else if(pid == 0) { // child process	
		printf("clfd = %d \n",clfd);			
		if(dup2(clfd,STDIN_FILENO) != STDIN_FILENO ||
				dup2(clfd,STDOUT_FILENO) != STDOUT_FILENO || 
					dup2(clfd,STDERR_FILENO) != STDERR_FILENO){
			do_error("dup2 error");							
		}	
		close(clfd);
		do_request();
		
		exit(0);	
	}else{
		// This wait() is to adopt the zombie processes		
		waitpid(-1,&status,WNOHANG);
		close(clfd);
	}
	return 0;		
}

int main(int argc, char *argv[]) {
	int port;
	int serv_sock;
	struct sockaddr_in serv_addr;

	if(argc != 2){
		printf("Please enter:	%s 8080 \n",argv[0]);
		return -1;
	}

	port = atoi(argv[1]);
	printf("Server is running ...... \n");
	printf("%p %p\n",sbuf,&fptr);

	// Ubuntu12.04, withou ASLR and stack canary, system("/bin/sh"); 
	//	((FUNPTR) 0xb7e5e430)((char *)0xb7f80d98);

	if((serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ) {
		do_error("Socket() failed");
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr));		
	serv_addr.sin_family =  AF_INET;			
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	
	serv_addr.sin_port = htons(port);
	
	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0 ){
		do_error("bind() failed");
	}

	if (listen(serv_sock, MAXPENDING) < 0 ){
		do_error("listen() failed");
	}

	for (;;) {					
		struct sockaddr_in client_addr;				
		unsigned int addr_len = sizeof(client_addr);		
		char client_name[INET_ADDRSTRLEN];	
		int client_sock;
		
		client_sock = accept(serv_sock, (struct sockaddr*) &client_addr, &addr_len);
		if ( client_sock < 0 ){
			do_error("accept() failed");
		}
		inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_name, sizeof(client_name));
		printf("Hello: %s/%d\n", client_name, ntohs(client_addr.sin_port));
		printf("serv_sock = %d , client_sock = %d \n",serv_sock,client_sock);		
		service(client_sock);		
		//printf("Byebye: %s/%d\n", client_name, ntohs(client_addr.sin_port));
	}
}



