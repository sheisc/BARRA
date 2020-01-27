/*****************************************************************
	
******************************************************************/
#include <unistd.h>
#include <dlfcn.h> 
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>


#define	MAIN_STACK_END	((unsigned long)__libc_csu_init)
#define	OTHER_STACK_END	0

#define	 GET_STACK_BASE_POINTER() ({ \
	unsigned long  p; \
	__asm__ __volatile__ (	\
        "movq %%rbp,%0 \r\n"	\
		:"=r"(p) \
		:	\
		:	\
	);	\
	p; \
})



void __libc_csu_init(void);



/////////////////////////////////////////////////////////////////////////////////////////////////



static void update_ret_id(unsigned long rbp){
	while(rbp != MAIN_STACK_END && rbp != OTHER_STACK_END ){
		printf("rbp = %lx \n",rbp);
		printf("ret = %lx \n\n", *((unsigned long *)(rbp + sizeof(unsigned long))) );
		rbp = *((unsigned long *)rbp);
	}
	
}


unsigned long au_edu_unsw_randomize(){
	unsigned long from, to;
	printf("au_edu_unsw_randomize() called before the hooked functions\n");
	
	unsigned long  rbp =	GET_STACK_BASE_POINTER();
	update_ret_id(rbp);
		
	return 0;
}

















