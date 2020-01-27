#include "WPA/WPAPass.h"

#include "llvm/Support/Debug.h"
#include "Util/BasicTypes.h"
#include "llvm/IR/BasicBlock.h"

#include "Util/AnalysisUtil.h"
#include "Util/GEPTypeBridgeIterator.h"
#include "MemoryModel/MemModel.h"
#include "MemoryModel/PAGNode.h"

#include "MemoryModel/LocationSet.h"
#include <utility>

#include <unordered_set>
#include <llvm/Transforms/Utils/UnifyFunctionExitNodes.h>
#include "Random/randomize.h"

#include <llvm/IR/Value.h>

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <llvm/IR/LLVMContext.h>

#if 1
#include "DDA/ContextDDA.h"
#include "DDA/FlowDDA.h"
#include "DDA/DDAClient.h"
#include "DDA/FlowDDA.h"
#include <llvm/Support/CommandLine.h>
#endif

using namespace llvm;
using namespace std;
/*
 (1) When instr2 in Project AsmRewriter is used,
    #define     UNSW_INSTR             0

 (2) When instr in Project AsmRewriter is used,
    #define     UNSW_INSTR             1

 */
#define     UNSW_INSTR             0

//#define OUTPUT(x)    do{ llvm::outs() << __FUNCTION__ << ":" << __LINE__ << "\n"; x; } while(0)
#define OUTPUT(x)    do{ x; } while(0)
#define DEBUG_OUTPUT(x)    do{ x; } while(0)

#define MAIN_FUNC_NAME              "main"
#define CXX_GLOBAL_SUB_I            "_GLOBAL__sub_I_"
#define CXX_GLOBAL_VAR_INIT         "__cxx_global_var_init"
#define ADDR_TAKEN_FUNC_PREFIX      ".unsw."

#define FUNCTION_ALIAS               8

#define FUNCTION_DECLS               7
#define FUNCTION_DEFS                6
#define DECLS_WITH_FUNC_PTRS         5
#define ADDRESS_TAKEN_DECLS          4
#define ADDRESS_TAKEN_DEFS           3
#define FUNC_WITH_JMP_RET        2
#define FUNC_WITH_RET                1
#define FUNC_WITH_JMP             0


#define KEEP_CALL          9
#define CONVERT_TO_JMP       10



//static cl::opt<string> functionsList("functions",  cl::init("functions.txt"),
//                                      cl::desc("Please specify the output file path"));

#include "../lib/Random/cxx_lib_names.txt"

static bool IsFuncTy(Type * ty, std::unordered_set<Type *> &visited){
    if(visited.count(ty) > 0){
        return false;
    }else{
        visited.insert(ty);
    }

    if(ty->isFunctionTy()){
        //DEBUG_OUTPUT(llvm::outs() << "-------------------------------" << *ty << "\n");
        return true;
    }else if(PointerType *pt = dyn_cast_or_null<PointerType>(ty)){
        return IsFuncTy(pt->getPointerElementType(),visited);
    }else if(ArrayType *at = dyn_cast_or_null<ArrayType>(ty)){
        return IsFuncTy(at->getArrayElementType(),visited);
    }else if(StructType *st = dyn_cast_or_null<StructType>(ty)){
        for(int i = 0; i < st->getNumElements(); i++){
            if(IsFuncTy(st->getElementType(i),visited)){
                return true;
            }
        }
        return false;
    }else{
        return false;
    }
}


// Test whether the function's parameters and return value contain function pointers.
static bool ContainsFuncPointer(const Function &f){
    //DEBUG_OUTPUT(llvm::outs() << f << "\n");

    std::unordered_set<Type *> visited;
    Type *retTy = f.getReturnType();
    if(IsFuncTy(retTy,visited)){
        return true;
    }
    for(Function::const_arg_iterator arg = f.arg_begin(), end = f.arg_end();
            arg != end; ++arg){
        Type * argTy = arg->getType();
        //DEBUG_OUTPUT(llvm::outs() << "-------------------------------" << *argTy << "\n");
        visited.clear();
        if(IsFuncTy(argTy,visited)){
            return true;
        }
    }
    return false;
}



void R4Analyzer::Analyze(llvm::Module &M){
    int addrTakenDefsCnt = 0, addrTakenDeclsCnt = 0;
    int totalCnt = 0;
    int declCnt = 0;
    int withDefCnt = 0;
    int directCallCnt = 0, indirectCallCnt = 0;
    int directInvokeCnt = 0, indirectInvokeCnt = 0;
    int retInstCnt = 0, unreachInstCnt = 0, otherTermCnt = 0;

    modName = M.getName();


    UnifyFunctionExitNodes* oneExitPass = new UnifyFunctionExitNodes();

    // const char * cxx_names[] is defined in "../lib/Random/cxx_lib_names.txt"
    for(unsigned i = 0; i < sizeof(cxx_names)/sizeof(cxx_names[0]); i++){
        cxxLibFuncNames.insert(std::string(cxx_names[i]));
    }

#if 1
    /*
        #include <stdio.h>
        #include <stdlib.h>

        extern "C" void f(){
            printf("f()\n");
        }
        extern "C" void g() __attribute__ ((weak, alias("f")));

        int main(){
            f();
            g();
        }
    */
    /// GlobalAlias
    for (Module::alias_iterator it = M.alias_begin(),
                 eit = M.alias_end(); it != eit; ++it) {
        GlobalAlias *alias = &*it;
        if(alias->getValueType()->isFunctionTy()){
            alias->replaceAllUsesWith(alias->getAliasee());
        }
    }
#endif
    for(Function &func: M){
        if(!func.isDeclaration()){
            oneExitPass->runOnFunction(func);
            BasicBlock * retBlock = oneExitPass->getReturnBlock();
            BasicBlock * unreachBlock = oneExitPass->getUnreachableBlock();
            if (retBlock && dyn_cast<ReturnInst>(retBlock->getTerminator())){
                retInstCnt++;
            }else if(unreachBlock && dyn_cast<UnreachableInst>(unreachBlock->getTerminator())){
                unreachInstCnt++;
                DEBUG_OUTPUT(llvm::outs() << func.getName() << "\n");
            }else{
                otherTermCnt++;
            }
        }
    }
    //
    for(const Function &f: M){
        if(!f.isDeclaration()){
            if(f.hasAddressTaken()){
                addrTakenDefsCnt++;
                addrTakenDefs.insert(&f);
            }
            withDefCnt++;
            defs.insert(&f);
        }else{
            if(f.hasAddressTaken()){
                addrTakenDeclsCnt++;
                addrTakenDecls.insert(&f);
            }
            declCnt++;
            declFuns.insert(&f);
        }
        totalCnt++;
        for(const BasicBlock &b: f){

            for(const Instruction &i: b){

                if (const CallInst *cInst = dyn_cast<CallInst>(&i)) {
                    if(!(cInst->getCalledFunction())) { // 
                        indirectCallCnt++;
                    }else{
                        directCallCnt++;
                    }
                } else if (const InvokeInst *iInst = dyn_cast<InvokeInst>(&i)) {
                    if (!(iInst->getCalledFunction())) { // 
                        indirectInvokeCnt++;
                    }else{
                        directInvokeCnt++;
                    }
                }
            }
        }
    }
    DEBUG_OUTPUT(llvm::outs() << "The number of all functions is : " << totalCnt << "\n");
    DEBUG_OUTPUT(llvm::outs() << "The number of address-taken defintions is : " << addrTakenDefsCnt << "\n");
    DEBUG_OUTPUT(llvm::outs() << "The number of address-taken declarations is : " << addrTakenDeclsCnt << "\n");
    DEBUG_OUTPUT(llvm::outs() << "The number of declaration of functions is : " << declCnt << "\n\n");
    DEBUG_OUTPUT(llvm::outs() << "The number of function definitions is : " << withDefCnt << "\n\n");
    DEBUG_OUTPUT(llvm::outs() << "The number of direct calls is : " << directCallCnt << "\n\n");
    DEBUG_OUTPUT(llvm::outs() << "The number of indirect calls is : " << indirectCallCnt << "\n\n");
    DEBUG_OUTPUT(llvm::outs() << "The number of direct invokes is : " << directInvokeCnt << "\n\n");
    DEBUG_OUTPUT(llvm::outs() << "The number of indirect invokes is : " << indirectInvokeCnt << "\n\n");
    DEBUG_OUTPUT(llvm::outs() << "The number of return instructions is : " << retInstCnt << "\n\n");
    DEBUG_OUTPUT(llvm::outs() << "The number of unreachable instructions is : " << unreachInstCnt << "\n\n");
    DEBUG_OUTPUT(llvm::outs() << "The number of other terminators is : " << otherTermCnt << "\n\n");

    DEBUG_OUTPUT(llvm::outs() << "\n********************* Declaration of functions ********************* \n");
    for(const Function * f: declFuns){
        DEBUG_OUTPUT(llvm::outs() << f->getName() << "\n");
    }

    DEBUG_OUTPUT(llvm::outs() << "\n********************* Declaration of functions which contain func pointers ********************* \n");
    for(const Function * f: declFuns){
        //DEBUG_OUTPUT(llvm::outs() << "Checking " << f->getName() << " .............\n");
        if(ContainsFuncPointer(*f)){
            declsWithFuncPtrs.insert(f);
            DEBUG_OUTPUT(llvm::outs() << *f << "\n");
        }
    }

    DEBUG_OUTPUT(llvm::outs() << "\n********************* Functions with definition ********************* \n");
    for(const Function * f: defs){
        DEBUG_OUTPUT(llvm::outs() << f->getName() << "\n");
        //OUTPUT(llvm::outs() << f->getName() << "\n");
    }

    DEBUG_OUTPUT(llvm::outs() << "\n********************* Address-taken function defintions ********************* \n");
    for(const Function * f: addrTakenDefs){
        DEBUG_OUTPUT(llvm::outs() << f->getName() << "\n");
    }

    DEBUG_OUTPUT(llvm::outs() << "\n********************* Address-taken function declarations ********************* \n");
    for(const Function * f: addrTakenDecls){
        DEBUG_OUTPUT(llvm::outs() << f->getName() << "\n");
    }

    AddQueries(M);



}

extern cl::bits<PointerAnalysis::PTATY> DDASelected;

void R4Analyzer::AddQuery(const llvm::Value *s){
  if(pta){
    PAG* pag = pta->getPAG();
    //
    if(pag->hasValueNode(s)){
      if(DDASelected.isSet(PointerAnalysis::Cxt_DDA)){
        //cout << "DDASelected.isSet(PointerAnalysis::Cxt_DDA)" << endl;
        ContextDDA * ctx = static_cast<ContextDDA *>(pta);
        ctx->GetDDAClient()->setQuery(pag->getValueNode(s));
      }else if(DDASelected.isSet(PointerAnalysis::FlowS_DDA)){
        FlowDDA * flow = static_cast<FlowDDA*>(pta);
        flow->GetDDAClient()->setQuery(pag->getValueNode(s));
      }
    }
  }

}

void R4Analyzer::HandleOneArg(const llvm::Value *arg,const llvm::Instruction * cInst){

    // CPU2006 462.libquantum
    // call i32 @atexit(void ()* bitcast (void (i8*)* @quantum_objcode_exit to void ()*))
    if(const BitCastOperator *bitcast = dyn_cast_or_null<BitCastOperator>(arg)){
        arg = bitcast->getOperand(0);
    }
    if(PointerType *pt = dyn_cast<PointerType>(arg->getType())){
        Type *ty = pt->getPointerElementType();
        if(ty->isFunctionTy()){
            this->AddQuery(arg);
            //pointers.insert(arg);
            //pointers.insert(std::make_pair<llvm::Value*,const llvm::Instruction*>((llvm::Value *)arg,cInst));
            pointers.insert(std::make_pair(arg,cInst));
        }else if(ty->isStructTy()){ // 

#if 1
            Value::const_user_iterator it = arg->user_begin(), e = arg->user_end();
            /*
                  %10 = getelementptr inbounds %struct.sigaction, %struct.sigaction* %6, i32 0, i32 0, !dbg !182
                  %11 = bitcast %union.anon* %10 to void (i32, %struct.siginfo_t*, i8*)**, !dbg !182
                  store void (i32, %struct.siginfo_t*, i8*)* @_ZL3hdliP9siginfo_tPv, void (i32, %struct.siginfo_t*, i8*)** %11, align 8, !dbg !183
                  %12 = getelementptr inbounds %struct.sigaction, %struct.sigaction* %6, i32 0, i32 2, !dbg !184
                  store i32 4, i32* %12, align 8, !dbg !185
                  %13 = call i32 @sigaction(i32 2, %struct.sigaction* %6, %struct.sigaction* null) #7, !dbg !186
             */
            for(; it != e; ++it){
                //llvm::GEPOperator
                const llvm::GetElementPtrInst *gep = dyn_cast_or_null<const GetElementPtrInst >(*it);
                if(gep){
                    const Instruction *cur = gep;
                    const llvm::BitCastInst *bitcast = dyn_cast_or_null<const BitCastInst>(gep->getNextNode());
                    if(bitcast){
                        cur = bitcast;
                    }
                    const llvm::StoreInst *store = dyn_cast_or_null<const StoreInst>(cur->getNextNode());
                    if(store){
                        const llvm::Value *val = store->getValueOperand();
                        if(val->getType()->isPointerTy()){
                            this->AddQuery(val);
                            //pointers.insert(val);
                            pointers.insert(std::make_pair((Value*)val,store));
                        }
                    }
                }
            }
#endif
        }


    }

}

void R4Analyzer::HandleExtCallInst(const CallInst *cInst){
    for(unsigned i = 0; i < cInst->getNumArgOperands(); i++){
        Value *arg = cInst->getArgOperand(i);
        HandleOneArg(arg,cInst);
    }
}

void R4Analyzer::HandleExtInvokeInst(const InvokeInst *iInst){ //
    for(unsigned i = 0; i < iInst->getNumArgOperands(); i++){
        Value *arg = iInst->getArgOperand(i);
        HandleOneArg(arg,iInst);
    }
}

void R4Analyzer::AddQueries(llvm::Module &M){
    llvm::LLVMContext &C = M.getContext();
    llvm::Value * keepCall = llvm::ConstantInt::get(llvm::IntegerType::get(C,32),KEEP_CALL);
    llvm::Value * convertToJmp = llvm::ConstantInt::get(llvm::IntegerType::get(C,32),CONVERT_TO_JMP);
    llvm::MDNode * callMetaData = llvm::MDNode::get(C,llvm::ValueAsMetadata::get(keepCall));
    llvm::MDNode * jmpMetaData = llvm::MDNode::get(C,llvm::ValueAsMetadata::get(convertToJmp));

    for(const Function &f: M){
        for(const BasicBlock &b: f){
            for(const Instruction &i: b){
                if (const CallInst *cInst = dyn_cast<CallInst>(&i)) {
                    ((CallInst *)cInst)->setMetadata("KeepCallInfo",callMetaData);
                    //cInst->getMetadata()

                    Function *target = cInst->getCalledFunction();
                    if(!target) { // 
                        //this->AddQuery(cInst->getCalledValue());
                        //pointers.insert(cInst->getCalledValue());
                    }else{
                        if(declsWithFuncPtrs.find(target) != declsWithFuncPtrs.end()){
                            HandleExtCallInst(cInst);
                        }

                    }
                } else if (const InvokeInst *iInst = dyn_cast<InvokeInst>(&i)) {
                    Function *target = iInst->getCalledFunction();
                    if (!target) { // 
//                        const Value * val = iInst->getCalledValue();
//                        this->AddQuery(val);
//                        //pointers.insert(iInst->getCalledValue(),i);
//                        pointers.insert(std::make_pair((Value*)val,iInst));
                    }else{
                        if(defs.find(target) != defs.end()){
                            /*
                                void simulate(){
                                    try
                                    {
                                      checkTimeLimits(1);  // invoke checkTimeLimits(), which should NOT be "jmp checkTimeLimits"
                                    }
                                    catch (cTerminationException *e)
                                    {
                                        checkTimeLimits(0);
                                        return;
                                    }
                                }
                             */
#if UNSW_INSTR == 1
                            defsWithHybridRets.insert(target);
#endif
                        }
                        if(declsWithFuncPtrs.find(target) != declsWithFuncPtrs.end()){
                            HandleExtInvokeInst(iInst);
                        }
                    }
                }
            }
        }
    }


}

void R4Analyzer::DoPrint(const llvm::Value *pointer){
    NodeID nid = pta->getPAG()->getValueNode(pointer);
    PointsTo& pts = pta->getPts(nid);

    DEBUG_OUTPUT(llvm::outs() << *pointer << "\n\t ===================> \n{\n");
    if (pts.empty()) {
        DEBUG_OUTPUT(llvm::outs() << "Empty.\n");
    } else {
        int pointeeCnt = 0;
        for (PointsTo::iterator it = pts.begin(), eit = pts.end();
             it != eit; ++it){
            const PAGNode* n = pta->getPAG()->getPAGNode(*it);
            if(n->hasValue()){
                Type * ty = n->getValue()->getType();
                PointerType *pt = dyn_cast<PointerType>(ty);

//                if(ContextDDA * ctx = dyn_cast_or_null<ContextDDA>(pta)){
//                    ctx->GetDDAClient()->setQuery(*it);
//                    ctx->GetDDAClient()->answerQueries(pta);
//                    ctx->finalize();
//                    PointsTo& pts = pta->getPts(*it);
//                    DEBUG_OUTPUT(llvm::outs() << pts.count() << "\n");
//                }
//                nid = *it;
//                if(FlowSensitive * fspta = dyn_cast_or_null<FlowSensitive>(pta)){
//                    PointsTo& pts = pta->getPts(nid);
//                    DEBUG_OUTPUT(llvm::outs() << pts.count() << "\n");
//                }

                if(pt && dyn_cast<FunctionType>(pt->getPointerElementType())){
                    pointeeCnt++;
//                    DEBUG_OUTPUT(llvm::outs() << "(" << pointeeCnt << ")\n");
//                    DEBUG_OUTPUT(llvm::outs() << "\t" << *n << "\n\t" << analysisUtil::getSourceLoc(n->getValue())<< "\n\n");

                    Function *func = (Function *)n->getValue();
                    //DEBUG_OUTPUT(llvm::outs() << func->getName() << "\n");
                    if(defs.find(func) != defs.end()){
                        DEBUG_OUTPUT(llvm::outs() << func->getName() << ", func->getNumUses() is " << func->getNumUses() << "\n");
//                        if(func->getNumUses() == 1){
//                            // BUG.  movl func, %rdx
//                            defsWithRet.insert(func);
//                        }else{ // > 1
//                            defsWithHybridRets.insert(func);
//                        }
                        Value::user_iterator i = func->user_begin(), e = func->user_end();
                        for(; i != e; ++i){
                            const CallInst * call = dyn_cast_or_null<const CallInst >(*i);
                            if(!call || !call->getCalledFunction() || !call->getCalledFunction()->isDeclaration()){
                                break;
                            }
                        }
                        if(i == e){ // e.g. signal(SIGINT, func); signal(.., func);
                            defsWithRet.insert(func);
                        }else{
                            defsWithHybridRets.insert(func);
                        }

                    }
                }
            }
        }
        DEBUG_OUTPUT(llvm::outs() << "}\n\n");
    }
}

void R4Analyzer::PrintPts(){
    if(pta){

        for(const std::pair<const Value *, const Instruction*> &p: pointers){
            this->DoPrint(p.first);
        }
    }

    for(const Function *f: defs){
        if(f->getName().equals(MAIN_FUNC_NAME) || f->getName().startswith(CXX_GLOBAL_SUB_I)
                || f->getName().startswith(CXX_GLOBAL_VAR_INIT)){
            defsWithRet.insert(f);
        }
        else if(defsWithRet.find(f) == defsWithRet.end()
                && defsWithHybridRets.find(f) == defsWithHybridRets.end()){
#if UNSW_INSTR == 1
            defsWithoutRet.insert(f);
#else
            if(f->getLinkage() == GlobalValue::ExternalLinkage
                && cxxLibFuncNames.find(f->getName()) != cxxLibFuncNames.end()){
                defsWithRet.insert(f);
            }else{
                defsWithoutRet.insert(f);
            }
#endif
        }
    }
#if  UNSW_INSTR == 1
    // return the addr-taken declares
    for(const Function * f: addrTakenDecls){
        string name = f->getName();
        ((Function*)f)->setName(ADDR_TAKEN_FUNC_PREFIX+name);
    }
#endif

    ofstream funcsFile(modName+".functions.txt");



    //(0)
    //DEBUG_OUTPUT(llvm::outs() << "\n********************* Function definitions without ret ********************* \n");
    for(const Function * f: defsWithoutRet){
        string name = f->getName();
#if UNSW_INSTR == 1
        /*

        C++ allows operator-overloaded functions, which might be called by shared libraries.
            #0  operator new (m=32) at main.cpp:31
            #1  0x00007ffff7ae2f19 in operator new[](unsigned long) ()
               from /usr/lib/x86_64-linux-gnu/libstdc++.so.6
            #2  0x0000000000400792 in main () at main.cpp:35
        /////////////////////////////////////////////////////////////////

            void *operator new(size_t m){
                return malloc(m);
            }

            int main(){
                char * ptr = new char[32];
                *ptr = 'a';
                return 0;
            }
         */
        if(f->getLinkage() == GlobalValue::ExternalLinkage
            && cxxLibFuncNames.find(name) != cxxLibFuncNames.end()){
            name = ADDR_TAKEN_FUNC_PREFIX+name;
            ((Function*)f)->setName(name);
            DEBUG_OUTPUT(llvm::outs() << "cxxLibFuncNames overloaded: " << name + "\n");
        }
#endif
        OUTPUT(funcsFile << FUNC_WITH_JMP << "\t" << name << "\tFUNC_WITHOUT_RET" << "\n");
    }

    //(1)
    //DEBUG_OUTPUT(llvm::outs() << "\n********************* Function definitions with ret ********************* \n");
    for(const Function * f: defsWithRet){
        string name = f->getName();
        OUTPUT(funcsFile << FUNC_WITH_RET << "\t" << name << "\t FUNC_WITH_RET" << "\n");
    }
    //(2)
    //DEBUG_OUTPUT(llvm::outs() << "\n********************* Function definitions with hybrid rets ********************* \n");
    for(const Function * f: defsWithHybridRets){
        string name = f->getName();
        OUTPUT(funcsFile << FUNC_WITH_JMP_RET << "\t" << name << "\t FUNC_WITH_HYBRID_RETS" << "\n");
    }
    //(3)
    //DEBUG_OUTPUT(llvm::outs() << "\n********************* Address-taken function defintions ********************* \n");
    for(const Function * f: addrTakenDefs){
        string name = f->getName();
        OUTPUT(funcsFile << ADDRESS_TAKEN_DEFS << "\t" << name << "\t ADDRESS_TAKEN_DEFS" << "\n");
    }
    //(4)
    //DEBUG_OUTPUT(llvm::outs() << "\n********************* Address-taken function declarations ********************* \n");
    for(const Function * f: addrTakenDecls){
        string name = f->getName();
        OUTPUT(funcsFile << ADDRESS_TAKEN_DECLS << "\t" << name << "\t ADDRESS_TAKEN_DECLS" << "\n");
    }
    //(5)
    //DEBUG_OUTPUT(llvm::outs() << "\n********************* Declaration of functions which contain func pointers ********************* \n");
    for(const Function * f: declsWithFuncPtrs){
        string name = f->getName();
        OUTPUT(funcsFile << DECLS_WITH_FUNC_PTRS << "\t" << name << "\t DECLS_WITH_FUNC_PTRS" << "\n");
    }
    //(6)
    //DEBUG_OUTPUT(llvm::outs() << "\n********************* Functions with definition ********************* \n");
    for(const Function * f: defs){
        string name = f->getName();
        OUTPUT(funcsFile << FUNCTION_DEFS << "\t" << name << "\t FUNCTION_DEFS" << "\n");
    }
    //(7)
    //DEBUG_OUTPUT(llvm::outs() << "\n********************* Declaration of functions ********************* \n");
    for(const Function * f: declFuns){
        string name = f->getName();
        OUTPUT(funcsFile << FUNCTION_DECLS << "\t" << name << "\t FUNCTION_DECLS" << "\n");
    }

}





