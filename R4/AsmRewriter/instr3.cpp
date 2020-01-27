/*
    instr3 is used to instrument *.so (C/C++)
    The return address is replaced with return id OUT-OF-PLACE
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

#define MAX_LINE            8192
#define MAIN_FUNC_NAME      "\t.type\tmain,@function"
#define FUNC_PREFIX   "\t.type\t"
#define INDIRECT_CALL   "\tcallq\t*"
#define CXX_GLOBAL_SUB_I    "\t.type\t_GLOBAL__sub_I_"
#define ADDR_TAKEN_FUNC_PREFIX      ".unsw."



//#define MAIN_FUNC_NAME_T    "\t.type\tmain,\t@function"

#define FUNCTION_DECLS               7
#define FUNCTION_DEFS                6
#define DECLS_WITH_FUNC_PTRS         5
#define ADDRESS_TAKEN_DECLS          4
#define ADDRESS_TAKEN_DEFS           3

#define FUNC_WITH_HYBRID_RETS        2
#define FUNC_WITH_JMP_RET            FUNC_WITH_HYBRID_RETS

#define FUNC_WITH_RET                1

#define FUNC_WITHOUT_RET             0
#define FUNC_WITH_JMP                FUNC_WITHOUT_RET



static int seq_no = 1;
static int seq_start = 1;


static std::map<string,string> functionDecls;
static std::map<string,string> functionDefs;
static std::map<string,string> declsWithFuncPtrs;
static std::map<string,string> addrTakenDefs;
static std::map<string,string> addrTakenDecls;


static std::map<string,string> defsWithJmp;
static std::map<string,string> defsWithRet;
static std::map<string,string> defsWithJmpRet;


static void add_instrumentation(const char * input_file, const char *modified_file, const char *whitelist_file) {
    int i = 0;
    ifstream srcAsm(input_file);
    ofstream instrAsm(modified_file);
    ifstream ifs(whitelist_file);
    string line;

    while(getline(ifs,line)){
        stringstream ss(line);
        int type;
        string funcName;
        ss >> type >> funcName;
        if(type == FUNC_WITH_JMP){
            defsWithJmp.insert(std::make_pair<string,string>(funcName,funcName));
        }else if(type == FUNC_WITH_RET){
            defsWithRet.insert(std::make_pair<string,string>(funcName,funcName));
        }else if(type == FUNC_WITH_JMP_RET){
            defsWithJmpRet.insert(std::make_pair<string,string>(funcName,funcName));
        }
        else if(type == ADDRESS_TAKEN_DEFS){
            addrTakenDefs.insert(std::make_pair<string,string>(funcName,funcName));
        }
        else if(type == ADDRESS_TAKEN_DECLS){
            addrTakenDecls.insert(std::make_pair<string,string>(funcName,funcName));
        }
        else if(type == DECLS_WITH_FUNC_PTRS){
            declsWithFuncPtrs.insert(std::make_pair<string,string>(funcName,funcName));
        }
        else if(type == FUNCTION_DEFS){
            functionDefs.insert(std::make_pair<string,string>(funcName,funcName));
        }
        else if(type == FUNCTION_DECLS){
            functionDecls.insert(std::make_pair<string,string>(funcName,funcName));
        }
    }

    string curFuncName;
    while(getline(srcAsm,line)){
        // Determine which function defintion we are in
        if(line.compare(0,strlen(FUNC_PREFIX),FUNC_PREFIX) == 0){
            stringstream ss(line);
            string type, name, comment;
            ss >> type >> name >> comment;
            // "	.type	_Z9finializev,@function"
            curFuncName = name.substr(0,name.find_first_of(','));
        }
        // Handle call instruction
        if(line.compare(0,7,"\tcallq\t") == 0){
            string calledName(line.substr(7));

            if(calledName.find("@PLT") != string::npos){
                //cout << "calledName " << calledName << endl;
                calledName = calledName.substr(0,calledName.find("@PLT"));
                //cout << "calledName " << calledName << endl;
            }
#if 0
            // indirect call or call functions with definition
            // BUG: indirect invoke
            if(line.compare(0,strlen(INDIRECT_CALL),INDIRECT_CALL) == 0
                    || defsWithJmp.find(calledName) != defsWithJmp.end()){ 

//                instrAsm << "\tsubq\t$" << (SS_OFFSET) << ", %rsp" << endl;
//                instrAsm << "\tpushq\t.unsw.retid@GOTPCREL+" << seq_no*sizeof(long) << "(%rip)" << endl;
//                instrAsm << "\taddq\t$" << (SS_OFFSET+CPU_WORD_LENGTH) << ", %rsp" << endl;
                string regName;
                if(calledName.find("%rax") != string::npos){
                    regName = "%r11";
                }else{
                    regName = "%rax";
                }

                instrAsm << "\tmovq\t.unsw.retid@GOTPCREL" << "(%rip), " << regName <<  endl;
                instrAsm << "\taddq\t$" << seq_no*sizeof(long) << ", " << regName << endl;
                instrAsm << "\tmovq\t" << "(" << regName << "), " << regName << endl;
                instrAsm << "\tmovq\t"<< regName << ", " << -(SS_OFFSET+CPU_WORD_LENGTH) << "(%rsp)" << endl;


                instrAsm << line << endl;
                instrAsm << ".unsw.retaddr." << seq_no << ":" << endl;
                seq_no++;
            }else{
                instrAsm << line << endl;
            }
#endif
#if 1
            // indirect call or call functions with definition
            // BUG: indirect invoke
            if(line.compare(0,strlen(INDIRECT_CALL),INDIRECT_CALL) == 0
                    || defsWithJmp.find(calledName) != defsWithJmp.end()){ 

//                instrAsm << "\tsubq\t$" << (SS_OFFSET) << ", %rsp" << endl;
//                instrAsm << "\tpushq\t.unsw.retid@GOTPCREL+" << seq_no*sizeof(long) << "(%rip)" << endl;
//                instrAsm << "\taddq\t$" << (SS_OFFSET+CPU_WORD_LENGTH) << ", %rsp" << endl;
                string regName;
                if(calledName.find("%rax") != string::npos){
                    regName = "%r11";
                }else{
                    regName = "%rax";
                }

                instrAsm << "\tmovq\t.unsw.retid@GOTPCREL" << "(%rip), " << regName <<  endl;
                instrAsm << "\taddq\t$" << seq_no*sizeof(long) << ", " << regName << endl;
                instrAsm << "\tmovq\t" << "(" << regName << "), " << regName << endl;
                instrAsm << "\tmovq\t"<< regName << ", " << -(SS_OFFSET+CPU_WORD_LENGTH) << "(%rsp)" << endl;
            }
            instrAsm << line << endl;
            instrAsm << ".unsw.retaddr." << seq_no << ":" << endl;
            seq_no++;
#endif

        }
        // Handle ret instruction
        else if(line.compare(0,5,"\tretq") == 0){
            if(defsWithJmp.find(curFuncName) != defsWithJmp.end()){
//                instrAsm << "\tmovq\t" << -(SS_OFFSET) << "(\%rsp), %r11" << endl;
//                instrAsm << "\taddq\t$" << CPU_WORD_LENGTH << ", \%rsp" << endl;
//                instrAsm << "\tjmp\t*.unsw.retaddr@GOTPCREL(,\%r11,8)" << endl;

                instrAsm << "\tmovq\t" << -(SS_OFFSET) << "(%rsp), %r11" << endl;
                instrAsm << "\tandq\t$" << MAX_RET_ID_MASK << ", %r11" << endl;
                instrAsm << "\taddq\t$" << CPU_WORD_LENGTH << ", %rsp" << endl;
                instrAsm << "\tmovq\t.unsw.retaddr@GOTPCREL" << "(%rip), %r9" << endl;
                instrAsm << "\tjmp\t*(%r9,%r11,8)" << endl;
            }
            else{
                instrAsm << line << endl;
            }
        }
        // Handle other lines
        else{
            instrAsm << line << endl;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    instrAsm << "\t" << ".addrsig_sym .unsw.retaddr" << endl;
    instrAsm << "\t" << ".addrsig_sym .unsw.retid" << endl;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Declare global labels

//    //    .ident	"clang version 7.0.0 (tags/RELEASE_700/final)"
//    //	.section	".note.GNU-stack","",@progbits
//    //	.addrsig
////    instrAsm << "\t" << ".ident\t\"clang version 7.0.0 (tags/RELEASE_700/final)\"" << endl;
////    instrAsm << "\t" << ".section\t\".note.GNU-stack\",\"\",@progbits" << endl;
//    for(i = seq_start; i < seq_no; i++){
//        instrAsm << "\t" << ".addrsig_sym .unsw.retaddr." << i << endl;
//    }

    instrAsm << endl;
    instrAsm << ".text" << endl;
    for(i = seq_start; i < seq_no; i++){
        instrAsm << "\t.globl\t.unsw.retaddr." << i << endl;
        instrAsm << "\t.type\t.unsw.retaddr." << i << ",@function" << endl;
    }

    instrAsm << endl;
    /*
        extern void createShadowStack();


        static __attribute__((constructor(101))) void AU_EDU_UNSW_R4_INIT_SO(){
            createShadowStack();
        }

     ------------------------------------------------------------------------
    .text
        .p2align	4, 0x90
        .type	AU_EDU_UNSW_R4_INIT_SO,@function
    AU_EDU_UNSW_R4_INIT_SO:
        pushq	%rbp
        movq	%rsp, %rbp
        callq	createShadowStack
        popq	%rbp
        retq
    .L_unsw_func_end0:
        .size	AU_EDU_UNSW_R4_INIT_SO, .L_unsw_func_end0-AU_EDU_UNSW_R4_INIT_SO

        .section	.init_array.101,"aw",@init_array
        .p2align	3
        .quad	AU_EDU_UNSW_R4_INIT_SO

        .ident	"clang version 7.0.0 (tags/RELEASE_700/final)"
        .section	".note.GNU-stack","",@progbits
        .addrsig
        .addrsig_sym AU_EDU_UNSW_R4_INIT_SO
        .addrsig_sym createShadowStack
     */

    instrAsm << ".text" << endl;
    instrAsm <<"\t" << ".p2align	4, 0x90" << endl;
    instrAsm <<"\t" << ".type	AU_EDU_UNSW_R4_INIT_SO,@function" << endl;
    instrAsm << "AU_EDU_UNSW_R4_INIT_SO:" << endl;
    instrAsm <<"\t" << "pushq	%rbp" << endl;
    instrAsm <<"\t" << "movq	%rsp, %rbp" << endl;
    instrAsm <<"\t" << "callq	createShadowStack" << endl;
    instrAsm <<"\t" << "popq	%rbp" << endl;
    instrAsm <<"\t" << "retq" << endl;
    instrAsm << ".L_unsw_func_end0:" << endl;
    instrAsm <<"\t" << ".size	AU_EDU_UNSW_R4_INIT_SO, .L_unsw_func_end0-AU_EDU_UNSW_R4_INIT_SO" << endl;
    instrAsm <<"\t" << ".section	.init_array.101,\"aw\",@init_array" << endl;

    instrAsm <<"\t" << ".p2align	3" << endl;
    instrAsm <<"\t" << ".quad	AU_EDU_UNSW_R4_INIT_SO" << endl;
    instrAsm <<"\t" << ".ident	\"clang version 7.0.0 (tags/RELEASE_700/final)\"" << endl;
    instrAsm <<"\t" << ".section	\".note.GNU-stack\",\"\",@progbits" << endl;
    instrAsm <<"\t" << ".addrsig" << endl;
    instrAsm <<"\t" << ".addrsig_sym AU_EDU_UNSW_R4_INIT_SO" << endl;
    instrAsm <<"\t" << ".addrsig_sym createShadowStack" << endl;


}


/* Main entry point */

int main(int argc, char** argv) {

    if (argc < 5) {
        perror("\n"
             "instr input.s output.s whitelist.txt retid.txt\n\n"
             );
        exit(1);
    }
    if(access(argv[4], F_OK) != -1){ // existing
      ifstream idFile(argv[4]);
      string line;
      if(getline(idFile,line)){
        stringstream ss(line);
        ss >> seq_no;
        seq_start = seq_no;
        cout << "seq_no = " << seq_no << endl;
      }
    }
    //seq_no = atoi(argv[4]);
    add_instrumentation(argv[1],argv[2],argv[3]);
    ofstream retid(argv[4]);
    retid << seq_no << endl;
    return 0;

}

