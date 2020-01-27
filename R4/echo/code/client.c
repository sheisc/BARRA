#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include "util.h"


void request(int sock){
	
	char buf[512];	
	int n;	
	while(1){
		printf("$");
		fgets(buf, sizeof(buf), stdin);
		send( sock, &buf, strlen(buf), 0);
		memset(buf,0,sizeof(buf));
		recv(sock, buf, sizeof(buf), 0);
		printf("%s \n",buf);		
	}
}

int main(int argc, char **argv){
	return do_it(argc,argv,request);
}

