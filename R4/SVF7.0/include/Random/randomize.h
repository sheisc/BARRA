#ifndef RANDOMIZE_H
#define RANDOMIZE_H

#include "llvm/IR/Value.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/PassSupport.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Operator.h"

#include "MemoryModel/PointerAnalysis.h"
#include "MemoryModel/PAG.h"

#include <map>
#include <vector>
#include <set>
#include <unordered_set>
#include <string>


class R4Analyzer{
public:
  std::unordered_set<const llvm::Function*> declFuns;
  std::unordered_set<const llvm::Function*> defs;
  std::unordered_set<const llvm::Function*> addrTakenDefs;
  std::unordered_set<const llvm::Function*> addrTakenDecls;
  std::unordered_set<const llvm::Function*> declsWithFuncPtrs;
  std::unordered_set<const llvm::Function*> defsWithHybridRets;
  //
  std::unordered_set<const llvm::Function*> defsWithoutRet;
  std::unordered_set<const llvm::Function*> defsWithRet;


  std::map<const llvm::Value*,const llvm::Instruction*> pointers;

  std::unordered_set<std::string> cxxLibFuncNames;

  std::string modName;

  PointerAnalysis * pta;
  //
  R4Analyzer(PointerAnalysis * p = 0){
    pta = p;
  }
  //
  //void AddDDAQuery(llvm::Value * s);
  //
  void Analyze(llvm::Module &M);
  //
  void AddQueries(llvm::Module &M);
  //
  void AddQuery(const llvm::Value * s);
  //
  void PrintPts();
  //
  void DoPrint(const llvm::Value *pointer);
  //
  void HandleExtCallInst(const llvm::CallInst *cInst);
  //
  void HandleExtInvokeInst(const llvm::InvokeInst *iInst);
  //
  void HandleOneArg(const llvm::Value *arg,const llvm::Instruction * cInst);
};



#endif
