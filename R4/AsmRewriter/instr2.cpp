/*
    instr2 is used to instrument the executable (C/C++).
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

using namespace std;

#define MAX_LINE            8192
#define MAIN_FUNC_NAME      "\t.type\tmain,@function"
#define FUNC_PREFIX   "\t.type\t"
#define INDIRECT_CALL   "\tcallq\t*"
#define CXX_GLOBAL_SUB_I    "\t.type\t_GLOBAL__sub_I_"
#define ADDR_TAKEN_FUNC_PREFIX      ".unsw."

#define CFI_STARTPROC       "\t.cfi_startproc"

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






//#define SS_OFFSET       (16L << 20)

//#define SEQ_NO_ORIGIN   0

static int seq_no = 1;
static int seq_start = 1;

//static int unsw_jbe_no = 0;
//static std::map<string,string> withDefFuncs;

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
        //cout << type << "\t" << funcName << endl;
        //withDefFuncs.insert(std::make_pair<string,string>(funcName,funcName));
    }

    string curFuncName = ".NO.EXSIT.FUNC.NAME";
    while(getline(srcAsm,line)){


        // Determine which function defintion we are in
        if(line.compare(0,strlen(FUNC_PREFIX),FUNC_PREFIX) == 0){
            stringstream ss(line);
            string type, name, comment;
            ss >> type >> name >> comment;
            // "	.type	_Z9finializev,@function"
            curFuncName = name.substr(0,name.find_first_of(','));
        }
        // Handle a line of assembly code
        if( (defsWithRet.find(curFuncName) != defsWithRet.end() || defsWithJmpRet.find(curFuncName) != defsWithJmpRet.end())
            &&
           line.compare(0,strlen(CFI_STARTPROC),CFI_STARTPROC) == 0){ // the start of a callback function
            instrAsm << line << endl;
            instrAsm << "\tpopq\t" << -(SS_OFFSET) << "(%rsp)" << endl;
            instrAsm << "\tsubq\t$8, " << "%rsp" << endl;

        }
#if 1
        else if(line.compare(0,strlen(CFI_STARTPROC),CFI_STARTPROC) == 0){ // the start of a jump-ret function
            instrAsm << line << endl;
            string regName = "%rax";
            instrAsm << "\taddq\t" << ".unsw.randomval, " << regName << endl;
            instrAsm << "\tandq\t$" << MAX_RET_ID_MASK << ", " << regName <<  endl;
            instrAsm << "\tmovq\t"<< regName << ", " << -(SS_OFFSET) << "(%rsp)" << endl;
            continue;
        }
#endif
        // Handle call instruction
        else if(line.compare(0,7,"\tcallq\t") == 0){
            string calledName(line.substr(7));

            if(calledName.find("@PLT") != string::npos){
                /*
                    
                    Never use find_last_of() here.
                 */
                calledName = calledName.substr(0,calledName.find("@PLT"));
            }

            // indirect call or call functions with definition
            if(line.compare(0,strlen(INDIRECT_CALL),INDIRECT_CALL) == 0
                    || defsWithJmp.find(calledName) != defsWithJmp.end()){ // FIXME:  indirect call should be CONSERVATIVE
#if 0
                string regName;
                if(calledName.find("%rax") != string::npos){
                    regName = "%r11";
                }else{
                    regName = "%rax";
                }
                // BUG   when "call *rax"
                //instrAsm << "\tmovq\t.unsw.retid+" << seq_no*sizeof(long) << "(\%rip), " << regName << endl;
                instrAsm << "\tmovq\t$" << seq_no << ", " << regName << endl;
                instrAsm << "\taddq\t" << ".unsw.randomval, " << regName << endl;
                instrAsm << "\tandq\t$" << MAX_RET_ID_MASK << ", " << regName <<  endl;
                instrAsm << "\tmovq\t"<< regName << ", " << -(SS_OFFSET+CPU_WORD_LENGTH) << "(%rsp)" << endl;
#endif
#if 1
                if(calledName.find("%rax") != string::npos){
                    instrAsm << "\tmovq\t" << "%rax, %r11" << endl;
                    line.replace(line.find("%rax"),4,"%r11");
                }
                instrAsm << "\tmovq\t$" << seq_no << ", %rax"<< endl;
#endif
            }

            instrAsm << line << endl;
            instrAsm << UNSW_RET_ADDR_PREFIX << seq_no << ":" << endl;
            seq_no++;
        }
        // Handle ret instruction
        else if(line.compare(0,5,"\tretq") == 0){
            if(defsWithJmp.find(curFuncName) != defsWithJmp.end()){
                instrAsm << "\tmovq\t" << -(SS_OFFSET) << "(%rsp), %r11" << endl;
                instrAsm << "\tsubq\t" << ".unsw.randomval, " << "%r11" << endl;
                instrAsm << "\tandq\t$" << MAX_RET_ID_MASK << ", %r11" << endl;
                instrAsm << "\taddq\t$" << CPU_WORD_LENGTH << ", %rsp" << endl;
                instrAsm << "\tjmp\t*.unsw.retaddr(,\%r11,8)" << endl;
            }
            else if(defsWithRet.find(curFuncName) != defsWithRet.end()
                      || defsWithJmpRet.find(curFuncName) != defsWithJmpRet.end()){ // the ret in callback function
              instrAsm << "\taddq\t$8, " << "%rsp" << endl;
              instrAsm << "\tpushq\t" << -(SS_OFFSET) << "(%rsp)" << endl;
              instrAsm << line << endl;
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
    instrAsm << endl;
    instrAsm << ".text" << endl;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    instrAsm << "\t" << ".addrsig_sym .unsw.retaddr" << endl;
    instrAsm << "\t" << ".addrsig_sym .unsw.retid" << endl;
    instrAsm << "\t" << ".addrsig_sym .unsw.randomval" << endl;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Declare global labels


    instrAsm << endl;
    instrAsm << ".text" << endl;
    for(i = seq_start; i < seq_no; i++){
        instrAsm << "\t.globl\t" << UNSW_RET_ADDR_PREFIX << i << endl;
        instrAsm << "\t.type\t" << UNSW_RET_ADDR_PREFIX << i << ",@function" << endl;
    }

}



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

