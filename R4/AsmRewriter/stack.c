/*****************************************************************

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

//#include <map>
//#include <iostream>
//using namespace std;

#include "randomize.h"

void __libc_csu_init(void);

#define DUMP_STACK_DEPTH_MAX 32
// The accumalative random value
//static int accuRandomVal;
static int curRandOffset;
static pid_t main_pid;

static int multiThreadMode = 0;



/////////////////////////////////////////////////////////////////////////////////////////////////

static void dump_trace() {
    void *stack_trace[DUMP_STACK_DEPTH_MAX] = {0};
    char **stack_strings = NULL;
    int stack_depth = 0;
    int i = 0;

    stack_depth = backtrace(stack_trace, DUMP_STACK_DEPTH_MAX);


    stack_strings = (char **)backtrace_symbols(stack_trace, stack_depth);
    if (NULL == stack_strings) {
        fprintf(stderr, "Out of memory!\n");
        return;
    }
    for (i = 0; i < stack_depth; ++i) {
        fprintf(stderr, " [%d] %s \r\n", i, stack_strings[i]);
    }

    free(stack_strings);
    stack_strings = NULL;
    return;
}

void rop_attack_catcher(void){
    fprintf(stderr, "\n################################ rop_attack_detected #################################\n\n");
    dump_trace();
    //exit(1);
}

/* Get unix time in microseconds */
unsigned long get_cur_time_us(void) {
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    return (tv.tv_sec * 1000000UL) + tv.tv_usec;
}






int au_edu_unsw_is_multi_thread_mode(){
    return multiThreadMode;
}

void au_edu_unsw_set_multi_thread_mode(int mode){
    multiThreadMode = mode;
}

pid_t au_edu_unsw_get_main_pid(){
    return main_pid;
}

void au_edu_unsw_set_main_pid(pid_t pid){
    main_pid = pid;
}

//RET_ID_TYPE * au_edu_unsw_get_ret_id_tbl(){
//    RET_ID_TYPE  * p = 0;
//    __asm__ __volatile__ (
//        "lea .unsw.retid,%0 \r\n"
//        :"=r"(p)
//        :
//        :
//    );
//    return p;
//}


RET_ADDR_TYPE * au_edu_unsw_get_ret_addr_tbl(){
    RET_ADDR_TYPE  * p = 0;
    __asm__ __volatile__ (
        "lea .unsw.retaddr,%0 \r\n"
        :"=r"(p)
        :
        :
    );
    return p;
}

RET_ADDR_TYPE * au_edu_unsw_get_readonly_ret_addr_tbl(){
    RET_ADDR_TYPE  * p = 0;
    __asm__ __volatile__ (
        "lea .unsw.retaddr.bak,%0 \r\n"
        :"=r"(p)
        :
        :
    );
    return p;
}

unsigned long au_edu_unsw_get_cnt_of_ret_ids(){
    unsigned long  cnt = 0;
    __asm__ __volatile__ (
        "movq .unsw.retid.cnt,%0 \r\n"
        :"=r"(cnt)
        :
        :
    );
    return cnt;
}

RET_ID_TYPE * au_edu_unsw_get_addr_of_random_val(){
    RET_ID_TYPE  * p = 0;
    __asm__ __volatile__ (
        "lea .unsw.randomval,%0 \r\n"
        :"=r"(p)
        :
        :
    );
    return p;
}



void update_one_stack(pid_t c_pid, unsigned long rbp){
    /*
     * For main thread:  getpid() == syscall(SYS_gettid)
     * For other pthreads:
     *                   getpid() is the pid of main thread
     *                   syscall(SYS_gettid) is the pid of current thread.     *
     */
    pid_t m_pid = au_edu_unsw_get_main_pid();
    while((m_pid == c_pid && rbp != MAIN_STACK_END) ||
          (c_pid != m_pid && rbp != OTHER_STACK_END) ){

        RET_ID_TYPE * pRetId = (RET_ID_TYPE *)(rbp + sizeof(RET_ID_TYPE) - SS_OFFSET);
        RET_ADDR_TYPE * pRetAddr;

        DEBUG_OUTPUT(pRetAddr = (RET_ADDR_TYPE *)(rbp + sizeof(RET_ADDR_TYPE)));
        *pRetId = (*pRetId + curRandOffset) % MAX_RET_ID_CNT;

        DEBUG_OUTPUT(printf("rbp = 0x%lx \n",rbp));
        DEBUG_OUTPUT(printf("ret = %p \n", *pRetAddr));
        DEBUG_OUTPUT(printf("retid = 0x%lx \n", *pRetId));
        DEBUG_OUTPUT(printf("curRandOffset = 0x%x \n\n", curRandOffset));


        rbp = *((unsigned long *)rbp);
    }
}

void update_random_value(){
    RET_ID_TYPE * pRandomVal = au_edu_unsw_get_addr_of_random_val();
    srandom (time (0));
    curRandOffset = random() % MAX_RET_ID_CNT;
    *pRandomVal = (*pRandomVal + curRandOffset) % MAX_RET_ID_CNT;
}


unsigned long au_edu_unsw_randomize(){
    if(au_edu_unsw_is_multi_thread_mode()){
        return 0;
    }

    unsigned long from, to;

    from = get_cur_time_us();

    // STEP 1:  update random value
    update_random_value();
    // STEP 2: update main stack
    /*
                        FIXME
        GET_RBP() had better be called in update_one_stack().
        OR
        make update_one_stack() be inline function or C Macro
        When stack.cpp is also instrumented.
     */
    unsigned long  rbp = GET_RBP();
    update_one_stack(syscall(SYS_gettid), rbp);

    to = get_cur_time_us();

    //printf("time used: %lu us\n",(to-from));
    DEBUG_OUTPUT(printf("time used: %lu us\n",(to-from)));
    return to - from;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////


#define unsw_error(format, ...)                                          \
    do {                                                            \
        fprintf(stderr, "error: " format "\n", ##__VA_ARGS__);      \
        abort();                                                    \
    } while (false)

//extern
static __attribute__((__noinline__))  void get_stack_bound(
        void **low_b, void **high_b){
    char *fptr0 = (char *)GET_RSP();
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


/*
    To make sure that createShadowStack() is called before any initialization
    of global C++ objects,
    we instrument the executable in stack.cpp or shared libraries in instr3.cpp to call
    createShadowStack().
    In this way, the stack of the main thread is expanded correctly.
 */
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

        {
            unsigned long cnt = au_edu_unsw_get_cnt_of_ret_ids();
            RET_ADDR_TYPE * roRetAddr = au_edu_unsw_get_readonly_ret_addr_tbl();
            RET_ADDR_TYPE * retAddr = au_edu_unsw_get_ret_addr_tbl();
            if(cnt > MAX_RET_ID_CNT){
                fprintf(stderr, "cnt > MAX_RET_ID_CNT\n");
                exit(0);
            }
            for(int i = 0; i < cnt; i++){
                retAddr[i] = roRetAddr[i];
            }
            for(int i = cnt; i < MAX_RET_ID_CNT; i++){
                retAddr[i] = (RET_ADDR_TYPE)(&rop_attack_catcher);
            }
            main_pid = getpid();
            // load-time randomization
            update_random_value();
            //update_ret_id();
            //ptrace(PTRACE_TRACEME, 0, NULL, NULL);
            //printf("main_pid = %lu\n",main_pid);
            //printf("main_pid = %lu\n",syscall(SYS_gettid));
            //dump_trace();
            //printf("cnt = %d\n",cnt);
            //printf("shadow stack created at expected location\n");
        }
        return;
    }
}



__attribute__((constructor(101))) void R4_init(){
    createShadowStack();
}


