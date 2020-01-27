#ifndef RANDOMIZE_H
#define RANDOMIZE_H


#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define	MAIN_STACK_END	((unsigned long)__libc_csu_init)
#define	OTHER_STACK_END	0

#define	 GET_RBP() ({ \
    unsigned long  p; \
    __asm__ __volatile__ (	\
        "movq %%rbp,%0 \r\n"	\
        :"=r"(p) \
        :	\
        :	\
    );	\
    p; \
})


#define	 GET_RSP() ({ \
    unsigned long  p; \
    __asm__ __volatile__ (	\
        "movq %%rsp,%0 \r\n"	\
        :"=r"(p) \
        :	\
        :	\
    );	\
    p; \
})

#define RET_ID_TYPE       unsigned long
#define RET_ADDR_TYPE     void *
#define MAX_RET_ID_CNT   (1 << 20)
#define MAX_RET_ID_MASK  (MAX_RET_ID_CNT - 1)
//#define PAGE_SIZE       4096
#define STACK_SIZE      (8 * (1 << 20))
// SS_OFFSET should be larger than STACK_SIZE
#define SS_OFFSET       (16L << 20)
//#define SS_OFFSET       (16L << 31)

// FIXME: 4 on 32-bit, 8 on 64-bit
// It should be signed integer, as "-8" might be used in assembly code.
#define CPU_WORD_LENGTH           ((long)(sizeof(unsigned long)))

#define DEBUG_OUTPUT(x)    do{ ; } while(0)

//#define UNSW_RET_ADDR_PREFIX    ".r."
#define UNSW_RET_ADDR_PREFIX   ".unsw.retaddr."
#ifdef __cplusplus
extern "C" {
#endif

    unsigned long au_edu_unsw_randomize(void);
    void createShadowStack(void);


    pid_t au_edu_unsw_get_main_pid();
    void au_edu_unsw_set_main_pid(pid_t pid);


    int au_edu_unsw_is_multi_thread_mode();
    void au_edu_unsw_set_multi_thread_mode(int mode);

#ifdef __cplusplus
}
#endif

#endif
















