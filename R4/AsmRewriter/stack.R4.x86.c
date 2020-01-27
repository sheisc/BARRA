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

//extern "C" void __libc_csu_init(void);


/*
    The MAIN_STACK_END in 32-bit subsystem on 64-bit Ubuntu is 0x00000000
    (gdb) si
    0x0804a68c in au_edu_unsw_randomize() ()
    (gdb) bt
    #0  0x0804a68c in au_edu_unsw_randomize() ()
    #1  0x0804a1db in f () at main.cpp:57
    #2  0x0804a21d in g () at main.cpp:60
    #3  0x0804a25d in h () at main.cpp:63
    #4  0x0804a2a4 in main () at main.cpp:67
    (gdb) frame 4
    #4  0x0804a2a4 in main () at main.cpp:67
    67	  h();
    (gdb) p $ebp
    $3 = (void *) 0xffffcd88
    (gdb) x /wx $ebp
    0xffffcd88:	0x00000000
 */
#define  MAIN_STACK_END_x86  0

#define DUMP_STACK_DEPTH_MAX 32
// The accumalative random value
static int accuRandomVal;
static int curRandOffset;


static int multiThreadMode = 0;

#define  GET_EBP() ({ \
    unsigned long  p; \
    __asm__ __volatile__ (  \
        "movl %%ebp,%0 \r\n"    \
        :"=r"(p) \
        :   \
        :   \
    );  \
    p; \
})

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
        //cerr << "[" << i << "] " << stack_strings[i] << endl;
        //perror(" [%d] %s \r\n", i, stack_strings[i]);
        fprintf(stderr, "[%d] %s\n",i,stack_strings[i]);
    }

    free(stack_strings);
    stack_strings = NULL;
    return;
}
/*
 * FIXME:   stack.R4.x86.cpp should also be proctected.
 * It is a callback.
 * For simplicity, we only call exit(1) to stop the attacked process.
 */
static void rop_attack_catcher(void){
    fprintf(stderr, "\n################################ rop_attack_detected #################################\n\n");
    //printf("\n################################ rop_attack_detected #################################\n\n");
    dump_trace();
    exit(1);
}


int au_edu_unsw_is_multi_thread_mode(){
    return multiThreadMode;
}

void au_edu_unsw_set_multi_thread_mode(int mode){
    multiThreadMode = mode;
}


static RET_ID_TYPE  * au_edu_unsw_get_ret_id_tbl(){
    RET_ID_TYPE  * p = 0;
    __asm__ __volatile__ (
        "lea .unsw.retid,%0 \r\n"
        :"=r"(p)
        :
        :
    );
    return p;
}


static RET_ADDR_TYPE  * au_edu_unsw_get_ret_addr_tbl() {
    RET_ADDR_TYPE  * p = 0;
    __asm__ __volatile__ (
        "lea .unsw.retaddr,%0 \r\n"
        :"=r"(p)
        :
        :
    );
    return p;
}

static RET_ADDR_TYPE  * au_edu_unsw_get_readonly_ret_addr_tbl() {
    RET_ADDR_TYPE  * p = 0;
    __asm__ __volatile__ (
        "lea .unsw.retaddr.bak,%0 \r\n"
        :"=r"(p)
        :
        :
    );
    return p;
}

static unsigned long  au_edu_unsw_get_cnt_of_ret_ids() {
    unsigned long  cnt = 0;
    __asm__ __volatile__ (
        "movl .unsw.retid.cnt,%0 \r\n"
        :"=r"(cnt)
        :
        :
    );
    return cnt;
}


/* Get unix time in microseconds */
static unsigned long get_cur_time_us(void) {
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    return (tv.tv_sec * 1000000UL) + tv.tv_usec;
}


static void update_one_stack(){
    /*
     * For main thread:  getpid() == syscall(SYS_gettid)
     * For other pthreads:
     *                   getpid() is the pid of main thread
     *                   syscall(SYS_gettid) is the pid of current thread.     *
     */
    unsigned long rbp = GET_EBP();

    // It is diffrent from 64-bit system
    while(rbp != MAIN_STACK_END_x86){

        RET_ID_TYPE * pRetId = (RET_ID_TYPE *)(rbp + sizeof(RET_ID_TYPE) - SS_OFFSET);
        RET_ADDR_TYPE * pRetAddr;

        //DEBUG_OUTPUT(pRetAddr = (RET_ADDR_TYPE *)(rbp + sizeof(RET_ADDR_TYPE)));
        *pRetId = (*pRetId + curRandOffset) % MAX_RET_ID_CNT;

//        DEBUG_OUTPUT(printf("ebp = 0x%lx \n",rbp));
//        DEBUG_OUTPUT(printf("ret = %p \n", *pRetAddr));
//        DEBUG_OUTPUT(printf("retid = 0x%lx \n", *pRetId));
//        DEBUG_OUTPUT(printf("curRandOffset = 0x%x \n\n", curRandOffset));


        rbp = *((unsigned long *)rbp);
    }

}

static void update_ret_id(){

    RET_ID_TYPE * ret_id = au_edu_unsw_get_ret_id_tbl();
    RET_ADDR_TYPE * ro_ret_addr = au_edu_unsw_get_readonly_ret_addr_tbl();
    RET_ADDR_TYPE * ret_addr = au_edu_unsw_get_ret_addr_tbl();
    unsigned long cnt = au_edu_unsw_get_cnt_of_ret_ids();

    srandom (time (0));
    curRandOffset = random() % MAX_RET_ID_CNT;


    for(int i = 0; i < cnt; i++){
        ret_addr[ret_id[i]] = (void*)rop_attack_catcher;
    }

    for(int i = 0; i < cnt; i++){
        ret_id[i] = (ret_id[i] + curRandOffset) % MAX_RET_ID_CNT;
    }
    accuRandomVal = (accuRandomVal + curRandOffset) % MAX_RET_ID_CNT;
    for(int i = 0; i < cnt; i++){
        int index = (ret_id[i] + MAX_RET_ID_CNT - accuRandomVal) % MAX_RET_ID_CNT;
        //assert(index < cnt);
        ret_addr[ret_id[i]] = ro_ret_addr[index];
    }

}



unsigned long au_edu_unsw_randomize(void){
    if(au_edu_unsw_is_multi_thread_mode()){
        return 0;
    }
    unsigned long from, to;

    from = get_cur_time_us();

    // STEP 1:  update return-id table
    update_ret_id();
    // STEP 2: update main stack
    update_one_stack();

    to = get_cur_time_us();
    //DEBUG_OUTPUT(printf("time used: %lu us\n",(to-from)));
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
    char *fptr0 = (char *)GET_EBP();
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

        new_stack_low = (char *)mmap(new_stack_low, stack_size, PROT_READ | PROT_WRITE,
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
            update_ret_id();
            //rop_attack_catcher();
            //printf("shadow stack created at expected location\n");
        }
        return;
    }
}



__attribute__((constructor(101))) void R4_init(){
    createShadowStack();
}


