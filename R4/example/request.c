#include <stdio.h>
#include <stdlib.h>
#define BUFSIZE  1024
char *gets(char *s);

/**
	Vulnerable Function
	(1) Buffer overflow caused by gets()
	(2) Format string vulnerability deriving from incorrect use of printf()
 */
void read_and_echo(void){
	char buffer[BUFSIZE];	
	gets(buffer);
	printf(buffer);
	printf("\n");	
}

void do_request(void){
	int i = 3;
	while(i > 0){
		read_and_echo();		
		i--;
	}
}


