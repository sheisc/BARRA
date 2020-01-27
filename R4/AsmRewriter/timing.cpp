/********************************************************************************
 *                  Used to calculate the time needed for randomization
 *  Usage
 *      
 *      ./timing  
 *      
 *      
 *
 */
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/user.h>
#include <sys/mman.h>

extern "C" void __libc_csu_init(void);
#include "randomize.h"
#include <iostream>
using namespace std;




//#define MAX_RET_ID_CNT   (1 << 20)

//#define CUR_CNT  100000

//int cnt = CUR_CNT;//1024*8;

static void rop_attack_catcher(void){
  cout << "rop_attack_detected" << endl;
  exit(1);
}
static unsigned long get_cur_time_us(void) {
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    return (tv.tv_sec * 1000000UL) + tv.tv_usec;
}
static unsigned long ret_id[MAX_RET_ID_CNT];

static void * ro_ret_addr[MAX_RET_ID_CNT];
static void * ret_addr[MAX_RET_ID_CNT];
static int accuRandomVal;
static int curRandOffset;


//static void update_ret_id(int cnt){

//    srandom (time (0));
//    curRandOffset = random() % MAX_RET_ID_CNT;


//    for(int i = 0; i < cnt; i++){
//        ret_addr[ret_id[i]] = (void*)rop_attack_catcher;
//    }

//    for(int i = 0; i < cnt; i++){
//        ret_id[i] = (ret_id[i] + curRandOffset) % MAX_RET_ID_CNT;
//    }
//    accuRandomVal = (accuRandomVal + curRandOffset) % MAX_RET_ID_CNT;
//    for(int i = 0; i < cnt; i++){
//        int index = (ret_id[i] + MAX_RET_ID_CNT - accuRandomVal) % MAX_RET_ID_CNT;
//        //assert(index < cnt);
//        ret_addr[ret_id[i]] = ro_ret_addr[index];
//    }

//}


static void update_random_value(){
    int * pRandomVal = &accuRandomVal;
    srandom (time (0));
    curRandOffset = random() % MAX_RET_ID_CNT;
    *pRandomVal = (*pRandomVal + curRandOffset) % MAX_RET_ID_CNT;
}


static void update_one_stack(pid_t c_pid, unsigned long rbp){

    while(rbp != MAIN_STACK_END){
        RET_ID_TYPE * pRetId = (RET_ID_TYPE *)(rbp + sizeof(RET_ID_TYPE) - SS_OFFSET);
        *pRetId = (*pRetId + curRandOffset) % MAX_RET_ID_CNT;
        //cout << "rbp = " << rbp << endl;
        rbp = *((unsigned long *)rbp);
    }
}

static char pages[4096*8];



static unsigned long randomize(int cnt){
    unsigned long from, to;
    char * addr = (char *)(((unsigned long)pages & ~(0xFFF)) + 4096);
    //cout << hex << addr << endl;
    from = get_cur_time_us();
    update_random_value();
    mprotect(addr, 4096, PROT_WRITE);
    mprotect(addr, 4096, PROT_READ);
    unsigned long  rbp = GET_RBP();
    update_one_stack(syscall(SYS_gettid), rbp);
    to = get_cur_time_us();
    return to - from;
}

unsigned long tmp;

static unsigned long f(int depth, int cnt){
  char buf[64];
  memset(buf,0,sizeof(buf));
  if(depth <= 0){
    return randomize(cnt);
  }else{
    unsigned long t = f(depth-1,cnt);
    // only to use buf[].
    buf[0] = t;
    tmp = buf[0];
    return t;
  }
}

static int depth[] = {32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 8192*2,8192*4};
// callCnt is deprecated now, 
// since our mapping table is never changed (for efficiency) during our design.
static int callCnt[] = {1000, 10000, 100000, 200000, 300000, 400000,
                 500000, 600000, 700000, 800000, 900000, 1000000};

static unsigned long bssData[1024*1024];


int main(int argc, char * argv[]){
  int dNum = sizeof(depth)/sizeof(depth[0]);
  int cNum = sizeof(callCnt) / sizeof(callCnt[0]);
  // unsigned long [dNum][cNum]
  unsigned long * times = (unsigned long *)malloc(dNum * cNum * sizeof(unsigned long ));
  memset(times,0,dNum * cNum * sizeof(unsigned long ));

  unsigned long addr = (unsigned long)(((unsigned long)pages & ~(0xFFF)) + 4096);
  //cout << hex << (unsigned long)pages << endl;
  //cout << hex << addr << endl;
  int r = mprotect((char *)addr, 4096, PROT_WRITE);
  //cout << "r = " << dec << r << endl;
  r = mprotect((char *)addr, 4096, PROT_READ);
  //cout << "r = " << dec << r << endl;

  int repeat = 500;
  for(int n = 0; n < repeat; n++){
    for(int i = 0; i < dNum; i++){
      for(int j = 0; j < cNum; j++){
        times[i * cNum + j] += f(depth[i],callCnt[j]);
      }
    }
  }
  //
//  cout << "TIME";
//  for(int i = 0; i < dNum; i++){
//    cout << "\t" << depth[i];
//  }
//  cout << endl;


//  for(int j = 0; j < cNum; j++){
//      cout << callCnt[j] << "\t";
//      for(int i = 0; i < dNum; i++){
//         cout << times[i * cNum + j]/repeat << "\t";
//      }
//      cout << endl;
//  }
    //cout << "StackDepth" << "\t" << "Time" << endl; 
    for(int i = 1; i <= 8; i++){ // FIXME hardcoded
      cout << depth[i] << "\t" << times[i * cNum + 0]/repeat << endl;
    }
    cout << endl;
  free(times);
  return 0;
}



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

        //printf("shadow stack created at expected location\n");
        return;
    }
}



__attribute__((constructor(101))) void R4_init(){
    createShadowStack();
}
