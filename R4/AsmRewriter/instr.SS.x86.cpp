/*
    instr.ss32 generates assembly code for 32-bit shadow stack 
    Usage:
        see main().
 */
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

//FIXME
#define CFI_STARTPROC       "\t.cfi_startproc"

#define RETL_INSTRUCTION    "\tretl"

static void add_instrumentation(const char * input_file, const char *modified_file) {
    ifstream srcAsm(input_file);
    ofstream instrAsm(modified_file);

    string line;

    while(getline(srcAsm,line)){
        // Handle call instruction
        if(line.compare(0,strlen(CFI_STARTPROC),CFI_STARTPROC) == 0){ // the start of a function            
            instrAsm << line << endl;
            instrAsm << "\tpopl\t" << -(SS_OFFSET) << "(%esp)" << endl;
            instrAsm << "\tsubl\t$4, " << "%esp" << endl;
        }
        // Handle ret instruction
        else if(line.compare(0,strlen(RETL_INSTRUCTION),RETL_INSTRUCTION) == 0){
            instrAsm << "\taddl\t$4, " << "%esp" << endl;
            instrAsm << "\tpushl\t" << -(SS_OFFSET) << "(%esp)" << endl;                
            instrAsm << line << endl;            
        }     
        else{
            instrAsm << line << endl;
        }
    }
}


/* Main entry point */

int main(int argc, char** argv) {

    if (argc < 3) {
        perror("\n"
             "instr.SS.x86 input.s output.s\n\n"
             );
        exit(1);
    }

    add_instrumentation(argv[1],argv[2]);

    return 0;

}

