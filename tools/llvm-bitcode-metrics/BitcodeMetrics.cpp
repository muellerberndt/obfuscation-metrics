//===----------------------------------------------------------------------===//
//
// This file defines a bitcode aggregating pass
//
// usage: opt -load LLVMBitcodeMetrics.so -analyze -bitcode-metrics a.ll
//
//===----------------------------------------------------------------------===//

#include "llvm/Pass.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Compression.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Bitcode/ReaderWriter.h"


/*****************************************
 * Pass implementation
 *****************************************/

using namespace llvm;

namespace {


class BitcodeMetrics : public ModulePass {

  size_t BasicBlocksCount_;
  size_t CFGEdgeCount_;
  size_t CGEdgeCount_;
  size_t DataFlowComplexity_;
  size_t FunctionCount_;
  size_t InstructionCount_;
  size_t ReturnCount_;
  size_t Kolmogorov_;

public:
  static char ID;

  BitcodeMetrics() : ModulePass(ID)
  {}

  bool runOnModule(Module &M) override {
    initStats();
    computeStats(M);
    return false;
  }

private:

  void runOnFunction(Function const & F) {
    BasicBlocksCount_ += F.size();
    for (BasicBlock const& BB: F) {
      runOnBasicBlock(BB);
    }
  }

  void runOnBasicBlock(BasicBlock const & BB) {
    InstructionCount_ += BB.size();
    CFGEdgeCount_ += BB.getTerminator()->getNumSuccessors();
    for (Instruction const& I: BB) {
      runOnInstruction(I);
    }
    SmallSet<Value const*, 32> Locals;
    SmallSet<Value const*, 32> Foreigns;
    for(Instruction const& I : BB) {
      for (Use const&U : I.operands()) {
        if (isa<BasicBlock>(U.get()))
          continue;
        if (isa<Constant>(U.get()))
          continue;
        if(Locals.count(U.get()))
          continue;
        Foreigns.insert(U.get());
      }
      Locals.insert(&I);
    }
    DataFlowComplexity_ += Foreigns.size();
  }
  void runOnInstruction(Instruction const& I) {
    if(isa<CallInst>(&I) || isa<InvokeInst>(&I))
      CGEdgeCount_ += 1;
    // count unreachable as a return as it's a terminator too
    // it's important because that's the terminator used after throwing an exception
    else if(isa<ReturnInst>(&I) || isa<UnreachableInst>(&I))
      ReturnCount_ += 1;

  }

  void initStats() {
    BasicBlocksCount_ =
    CFGEdgeCount_ =
    CGEdgeCount_ =
    DataFlowComplexity_ =
    FunctionCount_ =
    InstructionCount_ =
    ReturnCount_ =
    Kolmogorov_ =
    0;
  }
  void computeStats(Module const& M) {
    FunctionCount_ = M.size();
    std::for_each(M.begin(), M.end(), [this](Function const& F) { runOnFunction(F); });
    computeEntropy(M);
  }

  void computeEntropy(Module const& M) {
    std::string Bitcode;
    raw_string_ostream BitcodeStream(Bitcode);
    WriteBitcodeToFile(&M, BitcodeStream);


    SmallVector<char, 1> CompressedBitcode;
    zlib::compress(BitcodeStream.str(), CompressedBitcode, zlib::BestSizeCompression);
    Kolmogorov_ = CompressedBitcode.size();
  }

  void print(llvm::raw_ostream &O, llvm::Module const *) const override {

    size_t const CyclomaticCorrection = ReturnCount_ - FunctionCount_;

    O
      << "cg-edge-count:            " << CGEdgeCount_ << '\n'
      << "cfg-edge-count:           " << CFGEdgeCount_ << '\n'
      << "bb-count:                 " << BasicBlocksCount_ << '\n'
      << "instruction-count:        " << InstructionCount_ << '\n'
      << "cyclomatic-number:        " << (CFGEdgeCount_  + CyclomaticCorrection - BasicBlocksCount_ + 2 * FunctionCount_) << '\n'
      << "oviedo-complexity:        " << DataFlowComplexity_ + CFGEdgeCount_ << '\n'
      << "kolmogorov-complexity:    " << Kolmogorov_ << '\n'
      ;

  }
};
}

// pass registration is done through the constructor of static objects...

/* opt pass registration
 */
char BitcodeMetrics::ID = 0;
static RegisterPass<BitcodeMetrics>
    X("bitcode-metrics",  // the option name -> -mba
      "Various Bitcode Metrics", // option description
      true, // true as we don't modify the CFG
      true // true if we're writing an analysis
      );
