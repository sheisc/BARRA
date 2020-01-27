/*****************************************************************
    usage:
        LD_PRELOAD=/home/iron/src/R4/AsmRewriter/fork.x86.so ./main
******************************************************************/
#include <unistd.h>
#include <dlfcn.h> 
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>

//#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include <stdarg.h>
#include "randomize.h"
#include <sys/ptrace.h>

#include <map>
#include <iostream>
using namespace std;



/////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" char *gets(char *s);
extern "C" int puts(const char *s);

//int puts(const char *s){
//    typedef int (* PUTS_FUNC)(const char *s);
//    static PUTS_FUNC _puts;
//    if(!_puts){
//        _puts = (PUTS_FUNC) dlsym(RTLD_NEXT, "puts");
//        DEBUG_OUTPUT(_puts("puts() hooked.\n"));
//    }


//    au_edu_unsw_randomize();
//    return _puts(s);
//}

//char *gets(char *s){
//    typedef char * (* GETS_FUNC)(char *s);
//    static GETS_FUNC _gets;
//    if(!_gets){
//        _gets = (GETS_FUNC) dlsym(RTLD_NEXT, "gets");
//    }
//    // STDERR is not redirected to socket
//    DEBUG_OUTPUT(perror("gets() hooked.\n"));
//    //DEBUG_OUTPUT(perror("Runtime rerandomization before gets() in child process.\n"));
//    //au_edu_unsw_randomize();
//    return _gets(s);
//}

//ssize_t read(int fd, void *buf, size_t count){
//    typedef ssize_t (* READ_FUNC)(int fd, void *buf, size_t count);
//    static READ_FUNC _read;
//    if(!_read){
//        _read = (READ_FUNC) dlsym(RTLD_NEXT, "read");
//    }
//    DEBUG_OUTPUT(printf("read() hooked.\n"));
//    au_edu_unsw_randomize();
//    return _read(fd,buf,count);
//}


//ssize_t write(int fd, const void *buf, size_t count){
//    typedef ssize_t (* WRITE_FUNC)(int fd, const void *buf, size_t count);
//    static WRITE_FUNC _write;
//    if(!_write){
//        _write = (WRITE_FUNC) dlsym(RTLD_NEXT, "write");
//    }
//    DEBUG_OUTPUT(printf("write() hooked.\n"));
//    return _write(fd,buf,count);
//}

/////////////////////////////////////////////////////////////////////////////////////////////////
//void *dlopen(const char *filename, int flags){
//    typedef void * (*DLOPEN_FUNC)(const char *filename, int flags);
//    DLOPEN_FUNC _dlopen;

//    if(_dlopen){
//        _dlopen = (DLOPEN_FUNC) dlsym(RTLD_NEXT, "dlopen");
//    }
//    DEBUG_OUTPUT(printf("dloepn() hijacked.\n"));
//    return _dlopen(filename,flags);
//}


/////////////////////////////////////////////////////////////////////////////////////////////////

pid_t fork(void){
    typedef pid_t (* FORK_FUNC)(void);
    static FORK_FUNC _fork;

    if(!_fork){
        _fork = (FORK_FUNC) dlsym(RTLD_NEXT, "fork");
    }
    // fork() is called before IO redirection.
    // So it is OK to use printf() here.
    DEBUG_OUTPUT(printf("fork() hooked.\n"));
    pid_t pid = _fork();
    if(pid == 0){ // child process
        DEBUG_OUTPUT(printf("Runtime rerandomization after fork() in child process.\n"));
        au_edu_unsw_randomize();
    }
    return pid;
}


struct ArgInfo{
    void *(*start_routine) (void *);
    void *arg;
};

static void * do_start_routine(void *arg){
    struct ArgInfo * pArg = (struct ArgInfo *)arg;
    struct ArgInfo argInfo = *pArg;
    free(pArg);


    //DEBUG_OUTPUT(printf("do_start_routine(): 0x%lx\n", (unsigned long)(argInfo.start_routine)));
    //DEBUG_OUTPUT(printf("tid = %lu\n", (unsigned long) syscall(SYS_gettid)));
    //ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    void * retVal = (argInfo.start_routine)(argInfo.arg);
    return retVal;
}

/*
    One use of clone() is to implement threads:
        multiple threads of control in a program that run concurrently in a shared memory space.
    In Ubuntu16, each pthread corresponds to a process created via clone().
 */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg){
    typedef int (* PTHREAD_CREATE_FUNC)(pthread_t *thread, const pthread_attr_t *attr,
                              void *(*start_routine) (void *), void *arg);
    static PTHREAD_CREATE_FUNC _pthread_create;


    if(!_pthread_create){
        // The first call to pthread_create should be invoked by main thread.
        au_edu_unsw_set_multi_thread_mode(1);
        _pthread_create = (PTHREAD_CREATE_FUNC) dlsym(RTLD_NEXT, "pthread_create");
    }

    pthread_attr_t threadAttr;
    if(pthread_attr_init(&threadAttr) == -1){
        perror("error in pthread_attr_init()");
    }
    size_t stacksize = 0;
    if(attr){
        pthread_attr_getstacksize(attr, &stacksize);
    }else{
        //DEBUG_OUTPUT(printf("attr is NULL\n"));
        stacksize = STACK_SIZE;
    }
    //DEBUG_OUTPUT(printf("stacksize = 0x%x \n",stacksize));
    //FIXME
    stacksize += SS_OFFSET;
    if(!attr){
        attr = &threadAttr;
    }
    if(pthread_attr_setstacksize(( pthread_attr_t *)attr, stacksize) != 0){
        perror("error in pthread_attr_setstacksize()");
    }
    //DEBUG_OUTPUT(printf("stacksize = 0x%x \n",stacksize));
    //DEBUG_OUTPUT(printf("pthread_create(0x%x) is hijacked.\n",(unsigned long)start_routine));
    /*
        WARNING
        For thread safety, "struct ArgInof argInfo;" should NOT be used here.
        It might shared between the main thread and the pthread created by pthread_create().
     */
    struct ArgInfo * pArgInfo = (struct ArgInfo *) malloc(sizeof(struct ArgInfo));
    pArgInfo->start_routine= start_routine;
    pArgInfo->arg = arg;

    return _pthread_create(thread,attr,&do_start_routine,pArgInfo);

}
/////////////////////////////////////////////////////////////////////////////////////////////////

