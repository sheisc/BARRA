/*
    instr.x86 is used to instrument the executable (C/C++).
    The return address is replaced with return id OUT-OF-PLACE
    Usage:
        see main().
        // TODO
        It can be merged with instr2.cpp.
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
#define INDIRECT_CALL   "\tcalll\t*"
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
        if(line.compare(0,7,"\tcalll\t") == 0){
            string calledName(line.substr(7));

            if(calledName.find("@PLT") != string::npos){
                /*
                    
                    
                 */
                calledName = calledName.substr(0,calledName.find("@PLT"));
            }

            // indirect call or call functions with definition
            if(line.compare(0,strlen(INDIRECT_CALL),INDIRECT_CALL) == 0
                    || defsWithJmp.find(calledName) != defsWithJmp.end()){ // FIXME:  indirect call should be CONSERVATIVE

                instrAsm << "\tsubl\t$" << (SS_OFFSET) << ", %esp" << endl;
                instrAsm << "\tpushl\t.unsw.retid+" << seq_no*sizeof(RET_ID_TYPE) << endl;
                instrAsm << "\taddl\t$" << (SS_OFFSET+CPU_WORD_LENGTH) << ", %esp" << endl;

                instrAsm << line << endl;
                instrAsm << ".unsw.retaddr." << seq_no << ":" << endl;
                seq_no++;
            }else{
                instrAsm << line << endl;
            }            
        }
        // Handle ret instruction
        else if(line.compare(0,5,"\tretl") == 0){
            if(defsWithJmp.find(curFuncName) != defsWithJmp.end()){
                instrAsm << "\tmovl\t" << -(SS_OFFSET) << "(%esp), %edx" << endl;
                instrAsm << "\taddl\t$" << 4 << ", %esp" << endl;
                instrAsm << "\tjmp\t*.unsw.retaddr(,\%edx,4)" << endl;
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

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Declare global labels


    instrAsm << endl;
    instrAsm << ".text" << endl;
    for(i = seq_start; i < seq_no; i++){
        instrAsm << "\t.globl\t.unsw.retaddr." << i << endl;
        instrAsm << "\t.type\t.unsw.retaddr." << i << ",@function" << endl;
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

