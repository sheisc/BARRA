/****************************************************************
		A hooker example
*****************************************************************/
#include <stdio.h>
char *gets(char *s);
char buf[1024];
int main(int argc, char **argv){	
	gets(buf);
	printf("%s\n",buf);
	return 0;
}
