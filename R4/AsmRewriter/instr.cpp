/*
    instr is used for instrumenting C programs.
    The return address is replaced with return id IN-PLACE,
    
    Please use instr2.cpp instead for both C/C++.

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

using namespace std;


#include <sys/wait.h>
#include <sys/time.h>

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

#define FUNC_WITH_JMP_RET            2
#define FUNC_WITH_RET                1
#define FUNC_WITH_JMP                0

static int seq_no = 0;
static int unsw_jbe_no = 0;
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
        if(line.compare(0,7,"\tcallq\t") == 0){
            string calledName(line.substr(7));
            // indirect call or call functions with definition
            // 
            if(line.compare(0,strlen(INDIRECT_CALL),INDIRECT_CALL) == 0
                    || defsWithJmp.find(calledName) != defsWithJmp.end()){ // FIXME:  indirect call should be CONSERVATIVE
                instrAsm << "\tpushq\t.unsw.retid+" << seq_no*sizeof(long) << "(\%rip)" << endl;
                instrAsm << "\tjmp\t" << calledName << endl;
                instrAsm << ".unsw.retaddr." << seq_no << ":" << endl;
                seq_no++;
            }
            else{ // call external functions
                if(addrTakenDecls.find(calledName) != addrTakenDecls.end()){
                    string funcName = calledName.substr(strlen(ADDR_TAKEN_FUNC_PREFIX));
                    instrAsm << "\tcallq\t" << funcName << endl;
                }else{
                    instrAsm << line << endl;
                }
            }
        }
        // Handle ret instruction
        else if(line.compare(0,5,"\tretq") == 0){
            if(defsWithJmp.find(curFuncName) != defsWithJmp.end()){
                //popq %r11
                //jmp	*.unsw.retaddr(,%r11,8)
                instrAsm << "\tpopq\t\%r11" << endl;
                instrAsm << "\tjmp\t*.unsw.retaddr(,\%r11,8)" << endl;
            }else if(defsWithRet.find(curFuncName) != defsWithRet.end()){
                instrAsm << line << endl;
            }else if(defsWithJmpRet.find(curFuncName) != defsWithJmpRet.end()){//FIXME
                //popq %r11
                //cmpl	$0x100000, %r11
                //jbe   .unsw.jbe
                //jmp	*%r11
                //.unsw.jbe:
                //jmp   *.unsw.retaddr(,%r11,8)
                instrAsm << "\tpopq\t\%r11" << endl;
                instrAsm << "\tcmpq\t$0x100000, \%r11" << endl;
                instrAsm << "\tjbe\t.unsw.jbe." << unsw_jbe_no << endl;
                instrAsm << "\tjmp\t*\%r11" << endl;
                instrAsm << ".unsw.jbe." << unsw_jbe_no << ":" << endl;
                instrAsm << "\tjmp\t*.unsw.retaddr(,\%r11,8)" << endl;
                unsw_jbe_no++;
            }else{
                instrAsm << line << endl;
            }
        }
        // Handle other lines
        else{
            instrAsm << line << endl;
        }
    }
    instrAsm << endl;

    /*
    .text
    .globl	.unsw.func
        .p2align	4, 0x90
        .type	.unsw.func,@function
    .unsw.func:
        call func
        popq %r11
        jmp	*.unsw.retaddr(,%r11,8)
     */
    instrAsm << ".text" << endl;
    for(std::map<string,string>::iterator it = addrTakenDecls.begin(), e = addrTakenDecls.end(); it != e; ++it){
        string funcName = it->first.substr(strlen(ADDR_TAKEN_FUNC_PREFIX));
        instrAsm << ".global\t" << it->first << endl;
        instrAsm << "\t.p2align\t4, 0x90" << endl;
        instrAsm << "\t.type\t" << it->first << ",@function" << endl;
        instrAsm << it->first << ":" << endl;
        instrAsm << "\t" << "call\t" << funcName << endl;

        //popq %r11
        //cmpl	$0x100000, %r11
        //jbe   .unsw.jbe
        //jmp	*%r11
        //.unsw.jbe:
        //jmp   *.unsw.retaddr(,%r11,8)
        instrAsm << "\tpopq\t\%r11" << endl;
        instrAsm << "\tcmpq\t$0x100000, \%r11" << endl;
        instrAsm << "\tjbe\t.unsw.jbe." << unsw_jbe_no << endl;
        instrAsm << "\tjmp\t*\%r11" << endl;
        instrAsm << ".unsw.jbe." << unsw_jbe_no << ":" << endl;
        instrAsm << "\tjmp\t*.unsw.retaddr(,\%r11,8)" << endl;
        unsw_jbe_no++;
#if 0
        if(defsWithHybridRets.find(it->first) != defsWithHybridRets.end()){
            //popq %r11
            //cmpl	$0x100000, %r11
            //jbe   .unsw.jbe
            //jmp	*%r11
            //.unsw.jbe:
            //jmp   *.unsw.retaddr(,%r11,8)
            instrAsm << "\tpopq\t\%r11" << endl;
            instrAsm << "\tcmpq\t$0x100000, \%r11" << endl;
            instrAsm << "\tjbe\t.unsw.jbe." << unsw_jbe_no << endl;
            instrAsm << "\tjmp\t*\%r11" << endl;
            instrAsm << ".unsw.jbe." << unsw_jbe_no << ":" << endl;
            instrAsm << "\tjmp\t*.unsw.retaddr(,\%r11,8)" << endl;
            unsw_jbe_no++;
        }else{
            instrAsm << "\tpopq\t\%r11" << endl;
            instrAsm << "\tjmp\t*.unsw.retaddr(,\%r11,8)" << endl;
        }
#endif
        instrAsm << endl;
    }

    instrAsm << endl;
    /*
    .text
    .globl	func
        .p2align	4, 0x90
        .type	func,@function
    func:
        pushq ...
        jmp .unsw.func
    .unsw.retaddr.XXX:
        retq
     */
    for(std::map<string,string>::iterator it = defsWithJmp.begin(), e = defsWithJmp.end(); it != e; ++it){
        if(it->first.find(ADDR_TAKEN_FUNC_PREFIX,0) == 0){ // .unsw.func
            string funcName = it->first.substr(strlen(ADDR_TAKEN_FUNC_PREFIX));

            instrAsm << ".global\t" << funcName << endl;
            instrAsm << "\t.p2align\t4, 0x90" << endl;
            instrAsm << "\t.type\t" << funcName << ",@function" << endl;
            instrAsm << funcName << ":" << endl;

            instrAsm << "\tpushq\t.unsw.retid+" << seq_no*sizeof(long) << "(\%rip)" << endl;
            instrAsm << "\tjmp\t" << it->first << endl;
            instrAsm << ".unsw.retaddr." << seq_no << ":" << endl;
            seq_no++;
            instrAsm << "\tretq" << endl;
            instrAsm << endl;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Declare global labels
    for(i = 0; i < seq_no; i++){
        instrAsm << "\t.globl\t.unsw.retaddr." << i << endl;
    }
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
    for(i = 0; i < seq_no; i++){
        instrAsm << "\t.quad\t" << i << endl;
    }
    instrAsm << "\t.size\t.unsw.retid, " << seq_no * sizeof(long) << endl;
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
    for(i = 0; i < seq_no; i++){
        instrAsm << "\t.quad\t.unsw.retaddr." << i << endl;
    }
    instrAsm << "\t.size\t.unsw.retaddr, " << seq_no * sizeof(long) << endl;

    instrAsm << endl;
}


/* Main entry point */

int main(int argc, char** argv) {

    if (argc < 4) {
        perror("\n"
             "instr input.s output.s whitelist.txt\n\n"
             );
        exit(1);
    }

    add_instrumentation(argv[1],argv[2],argv[3]);

    return 0;

}


