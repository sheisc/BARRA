#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include <sys/user.h>

#include <sys/wait.h>
#include <sys/time.h>
#include "randomize.h"
#include <unistd.h>

using namespace std;










static void gen_addr(int seq_start, int seq_end, int wordsize, char * path) {
    int i;

    ofstream instrAsm(path);

    instrAsm << ".text" << endl;



    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Declare global labels
    for(i = seq_start; i < seq_end; i++){
        instrAsm << "\t.globl\t" << UNSW_RET_ADDR_PREFIX << i << endl;
    }
#if 0
    /*
    .type	.unsw.retid, @object             # @.unsw.retid
    .data
    .globl	.unsw.retid
    .p2align	4
.unsw.retid:
    .quad	0                       # 0x0
    .quad	1                       # 0x1
    .quad	2                       # 0x2
    .quad	3                       # 0x3
    .size	.unsw.retid, 32
   */
    instrAsm << "\n\t.type\t.unsw.retid, @object" << endl;
    instrAsm << "\t.data" << endl;
    instrAsm << "\t.globl\t.unsw.retid" << endl;
    instrAsm << "\t.p2align\t4" << endl;
    instrAsm << ".unsw.retid:" << endl;


    //
    for(i = 0; i < seq_start; i++){
        instrAsm << "\t.quad\t" << 0 << endl;
    }
    for(i = seq_start; i < seq_end; i++){
        instrAsm << "\t.quad\t" << i << endl;
    }
    instrAsm << "\t.size\t.unsw.retid, " << seq_end * sizeof(RET_ID_TYPE) << endl;
#endif
//    if(seq_end >= MAX_RET_ID_CNT){
//        instrAsm << "\t.size\t.unsw.retid, " << seq_end * sizeof(RET_ID_TYPE) << endl;
//    }else{
//        instrAsm << "\t.zero\t" << (MAX_RET_ID_CNT - seq_end) * sizeof(RET_ID_TYPE) << endl;
//        instrAsm << "\t.size\t.unsw.retid, " << MAX_RET_ID_CNT * sizeof(RET_ID_TYPE) << endl;
//    }

#if 0   // NOT BSS
    /*
    .type	.unsw.retaddr,@object            # @.unsw.retaddr
    .globl	.unsw.retaddr
    .p2align	4
.unsw.retaddr:
    .quad	f
    .quad	.unsw.retaddr.1
    .quad	f
    .quad	f
    .size	.unsw.retaddr, 32
   */
    instrAsm << "\n\t.type\t.unsw.retaddr, @object" << endl;
    instrAsm << "\t.globl\t.unsw.retaddr" << endl;
    instrAsm << "\t.p2align\t4" << endl;
    instrAsm << ".unsw.retaddr:" << endl;
    //
    for(i = 0; i < seq_start; i++){
        instrAsm << "\t.quad\t" << 0 << endl;
    }
    for(i = seq_start; i < seq_end; i++){
        instrAsm << "\t.quad\t" << UNSW_RET_ADDR_PREFIX << i << endl;
    }
    if(seq_end >= MAX_RET_ID_CNT){
        instrAsm << "\t.size\t.unsw.retaddr, " << seq_end * sizeof(RET_ADDR_TYPE) << endl;
    }else{
        // .zero	8168
        instrAsm << "\t.zero\t" << (MAX_RET_ID_CNT - seq_end) * sizeof(RET_ADDR_TYPE) << endl;
        instrAsm << "\t.size\t.unsw.retaddr, " << MAX_RET_ID_CNT * sizeof(RET_ADDR_TYPE) << endl;
    }

    instrAsm << endl;
#endif

#if 1   //BSS
    /*
    .type	.unsw.retaddr,@object         # @bssData
        .globl	.unsw.retaddr
        .bss
        .p2align	4
    .unsw.retaddr:
        .zero	8388608
        .size	bssData, 8388608
    */
    instrAsm << "\n\t.type\t.unsw.retaddr, @object" << endl;
    instrAsm << "\t.globl\t.unsw.retaddr" << endl;
    instrAsm << "\t.bss" << endl;
    instrAsm << "\t.p2align\t4" << endl;
    instrAsm << ".unsw.retaddr:" << endl;
    instrAsm << "\t.zero\t" << MAX_RET_ID_CNT * sizeof(RET_ADDR_TYPE) << endl;
    instrAsm << "\t.size\t.unsw.retaddr, " << MAX_RET_ID_CNT * sizeof(RET_ADDR_TYPE) << endl;
    instrAsm << endl;
#endif
    /*
    .type	.unsw.retid.cnt, @object
    .data
    .globl	.unsw.retid.cnt
    .p2align	4
.unsw.retid.cnt:
    .quad   xxx
    .size	.unsw.retid.cnt, 8
     */
    instrAsm << "\t" << ".type	.unsw.retid.cnt, @object" << endl;
    instrAsm << "\t" << ".data" << endl;
    instrAsm << "\t" << ".globl	.unsw.retid.cnt" << endl;
    instrAsm << "\t" << ".p2align	4" << endl;
    instrAsm << ".unsw.retid.cnt:" << endl;
    instrAsm << "\t" << ".quad	" << seq_end << endl;
    instrAsm << "\t" << ".size	.unsw.retid.cnt, 8" << endl;

    /*
    .type	.unsw.randomval,@object         # @bssData
        .globl	.unsw.randomval
        .bss
        .p2align	12
    .unsw.randomval:
        .zero	PAGE_SIZE
        .size	.unsw.randomval, PAGE_SIZE
     */
    instrAsm << "\n\t.type\t.unsw.randomval, @object" << endl;
    instrAsm << "\t.globl\t.unsw.randomval" << endl;
    instrAsm << "\t.bss" << endl;
    instrAsm << "\t.p2align\t12" << endl;
    instrAsm << ".unsw.randomval:" << endl;
    instrAsm << "\t.zero\t" << PAGE_SIZE << endl;
    instrAsm << "\t.size\t.unsw.randomval, " << PAGE_SIZE << endl;

    /*
    .section	.rodata
    .type	.unsw.retaddr.bak,@object
    .globl	.unsw.retaddr.bak
    .p2align	4
.unsw.retaddr:
    .quad	f
    .quad	.unsw.retaddr.1
    .quad	f
    .quad	f
    .size	.unsw.retaddr, 32
   */
    instrAsm << "\n\t.section\t.rodata" << endl;
    instrAsm << "\t.type\t.unsw.retaddr.bak, @object" << endl;
    instrAsm << "\t.globl\t.unsw.retaddr.bak" << endl;
    instrAsm << "\t.p2align\t4" << endl;
    instrAsm << ".unsw.retaddr.bak:" << endl;
    //
    for(i = 0; i < seq_start; i++){
        instrAsm << "\t.quad\t" << 0 << endl;
    }
    for(i = seq_start; i < seq_end; i++){
        instrAsm << "\t.quad\t" << UNSW_RET_ADDR_PREFIX << i << endl;
    }
    instrAsm << "\t.size\t.unsw.retaddr.bak, " << seq_end * sizeof(RET_ADDR_TYPE) << endl;
    instrAsm << endl;
}


/* Main entry point */

int main(int argc, char** argv) {

    if (argc < 5) {
        perror("\n"
             "gen_addr seq_start seq_end wordsize asmfile\n\n"
             );
        exit(1);
    }
    //[seq_start)
    int seq_start = atoi(argv[1]);
    if(seq_start <= 0){
        seq_start = 1;
    }
    int seq_end = atoi(argv[2]);
    int wordsize = atoi(argv[3]);

    gen_addr(seq_start,seq_end,wordsize,argv[4]);

    return 0;

}

