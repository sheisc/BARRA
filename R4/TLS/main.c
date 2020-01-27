#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <asm/ldt.h>
#include <malloc.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

// thread local storage local value
/*
	.globl	barra_ranval
		.section	.tbss,"awT",@nobits
		.align 4
		.type	barra_ranval, @object
		.size	barra_ranval, 4
	barra_ranval:
		.zero	4
*/
extern __thread int barra_ranval;
//
#define	 GET_BARRA_RANVAL() ({ \
	int  p; \
	__asm__ __volatile__ (	\
		"movl	%%fs:barra_ranval@tpoff, %0 \r\n"	\
		:"=r"(p) \
		:	\
		:	\
	);	\
	p; \
})

#define	 SET_BARRA_RANVAL(val) ({ \
	int  p = (val); \
	__asm__ __volatile__ (	\
		"movl	%0, %%fs:barra_ranval@tpoff \r\n"	\
		: \
		:"r"(p)\
		:	\
	);	\
	p; \
})

#if 0 
int GET_BARRA_RANVAL(){
	int  p; 
	__asm__ __volatile__ (	
		"movl	%%fs:barra_ranval@tpoff, %0 \r\n"
		:"=r"(p) 
		:	
		:	
	);	
	return p; 
}
#endif


void t1(void){	
	int i = 5;
	while(i >= 0){
		barra_ranval = 30;
		printf("----- t1: thread local storage barra_ranval = %d \n", GET_BARRA_RANVAL());
		SET_BARRA_RANVAL(40);
		printf("----- t1: thread local storage barra_ranval = %d \n", barra_ranval);
		sleep(1);
		i--;
	}
}
void t2(void){
	int i = 5;
	barra_ranval = 50;
	while(i >= 0){
		printf("t2: thread local storage barra_ranval = %d \n", barra_ranval);
		sleep(1);
		i--;
	}	
}

int main(int argc, char **argv){
	int tls_base;  
	pthread_t tid;
	pthread_create(&tid,NULL,(void*)t1,NULL);
	pthread_create(&tid,NULL,(void*)t2,NULL);
	pthread_join(tid,NULL);
	return 0;
}





