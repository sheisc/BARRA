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
        instrAsm << "\t.globl\t.unsw.retaddr." << i << endl;
    }
    /*
    .type	.unsw.retid, @object             # @.unsw.retid
    .data
    .globl	.unsw.retid
    .p2align	4
.unsw.retid:
    .long	0                       # 0x0
    .long	1                       # 0x1
    .long	2                       # 0x2
    .long	3                       # 0x3
    .size	.unsw.retid, 16
   */
    instrAsm << "\n\t.type\t.unsw.retid, @object" << endl;
    instrAsm << "\t.data" << endl;
    instrAsm << "\t.globl\t.unsw.retid" << endl;
    instrAsm << "\t.p2align\t4" << endl;
    instrAsm << ".unsw.retid:" << endl;


    //
    for(i = 0; i < seq_start; i++){
        instrAsm << "\t.long\t" << 0 << endl;
    }
    for(i = seq_start; i < seq_end; i++){
        instrAsm << "\t.long\t" << i << endl;
    }
    instrAsm << "\t.size\t.unsw.retid, " << seq_end * sizeof(RET_ID_TYPE) << endl;


#if 1   //BSS
    /*
    .type	.unsw.retaddr,@object         # @bssData
        .globl	.unsw.retaddr
        .bss
        .p2align	4
    bssData:
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
#endif
    /*
    .type	.unsw.retid.cnt, @object
    .data
    .globl	.unsw.retid.cnt
    .p2align	4
.unsw.retid.cnt:
    .long   xxx
    .size	.unsw.retid.cnt, 4
     */
    instrAsm << "\t" << ".type	.unsw.retid.cnt, @object" << endl;
    instrAsm << "\t" << ".data" << endl;
    instrAsm << "\t" << ".globl	.unsw.retid.cnt" << endl;
    instrAsm << "\t" << ".p2align	4" << endl;
    instrAsm << ".unsw.retid.cnt:" << endl;
    instrAsm << "\t" << ".long	" << seq_end << endl;
    instrAsm << "\t" << ".size	.unsw.retid.cnt, " << sizeof(RET_ADDR_TYPE) << endl;


    /*
    .section	.rodata
    .type	.unsw.retaddr.bak,@object
    .globl	.unsw.retaddr.bak
    .p2align	4
.unsw.retaddr:
    .long	f
    .long	.unsw.retaddr.1
    .long 	f
    .long	f
    .size	.unsw.retaddr, 16
   */
    instrAsm << "\n\t.section\t.rodata" << endl;
    instrAsm << "\t.type\t.unsw.retaddr.bak, @object" << endl;
    instrAsm << "\t.globl\t.unsw.retaddr.bak" << endl;
    instrAsm << "\t.p2align\t4" << endl;
    instrAsm << ".unsw.retaddr.bak:" << endl;
    //
    for(i = 0; i < seq_start; i++){
        instrAsm << "\t.long\t" << 0 << endl;
    }
    for(i = seq_start; i < seq_end; i++){
        instrAsm << "\t.long\t.unsw.retaddr." << i << endl;
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

