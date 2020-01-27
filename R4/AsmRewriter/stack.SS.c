/*****************************************************************
    For 32-bit x86
******************************************************************/
//#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */

#include <dlfcn.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>



#include "randomize.h"



/////////////////////////////////////////////////////////////////////////////////////////////////




#define unsw_error(format, ...)                                          \
    do {                                                            \
        fprintf(stderr, "error: " format "\n", ##__VA_ARGS__);      \
        abort();                                                    \
    } while (false)

//extern
static __attribute__((__noinline__))  void get_stack_bound(
        void **low_b, void **high_b){
    char *fptr0 = (char *)GET_RBP();
    char *fptr = fptr0 - ((uintptr_t)fptr0 % PAGE_SIZE);
    *low_b = fptr;
    fptr += PAGE_SIZE;
    // mincore() will fail with ENOMEM for unmapped pages.  We can therefore
    // linearly scan to the base of the stack.
    // Note in practice this seems to be 1-3 pages at most if called from a
    // constructor.
    unsigned char vec;
    /*
        mincore() returns a vector that indicates
        whether pages of the calling process's virtual memory are resident in core (RAM)
     */
    while (mincore(fptr, PAGE_SIZE, &vec) == 0)
        fptr += PAGE_SIZE;
    if (errno != ENOMEM)
        unsw_error("failed to mincore page: %s", strerror(errno));
    *high_b = fptr;
    //printf("get_stack_high_b()\n");
    return;
}



void createShadowStack(void){
    static int init = 0;
    if(!init){
        char *stack_low, *stack_high, *new_stack_low;
        init = 1;
        //pthread_mutex_init(&mutex,NULL);

        get_stack_bound((void **)&stack_low, (void **)&stack_high);
        size_t stack_size = (size_t)stack_high - (size_t)stack_low;
        stack_size = (stack_size > STACK_SIZE) ? stack_size : STACK_SIZE;
        new_stack_low = stack_high - stack_size - SS_OFFSET;

        unsigned char vec;
        if (mincore(new_stack_low, stack_size, &vec) == 0)
            unsw_error("shadow stack region has been occupied");

        new_stack_low = (char *) mmap(new_stack_low, stack_size, PROT_READ | PROT_WRITE,
                             MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (new_stack_low == MAP_FAILED)
            unsw_error("failed to allocate stack: %s", strerror(errno));

        //printf("shadow stack created at expected location\n");
        return;
    }
}

__attribute__((constructor(101))) void R4_init(){
    createShadowStack();
}

