#include "PICMidInstructionSelector.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/CodeGen/GlobalISel/GIMatchTableExecutorImpl.h"
#include "llvm/CodeGen/GlobalISel/GenericMachineInstrs.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelector.h"
#include "llvm/CodeGen/GlobalISel/MIPatternMatch.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include "llvm/CodeGen/GlobalISel/Utils.h"
#include "llvm/CodeGen/LowLevelType.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Register.h"
#include "llvm/CodeGen/RegisterBankInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/ObjectYAML/MachOYAML.h"
#include "llvm/Support/ErrorHandling.h"

#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "PICMidInstrInfo.h"
#include "PICMidRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "picmid-isel"

namespace {

// #define GET_GLOBALISEL_PREDICATE_BITSET
// #include "PICMidGenGlobalISel.inc"
// #undef GET_GLOBALISEL_PREDICATE_BITSET

class PICMidInstructionSelector : public InstructionSelector {
public:
  PICMidInstructionSelector(const PICMidTargetMachine &TM, PICMidSubtarget &STI,
                            PICMidRegisterBankInfo &RBI);
  bool select(MachineInstr &I) override;

  static const char *getName() { return DEBUG_TYPE; }

private:
  const PICMidSubtarget &STI;
  const PICMidInstrInfo &TII;
  const PICMidRegisterInfo &TRI;
  const PICMidRegisterBankInfo &RBI;

  /// tblgen-erated 'select' implementation, used as the initial selector for
  /// the patterns that don't require complex C++.
  bool selectImpl(MachineInstr &MI, CodeGenCoverage &CoverageInfo) const;

  // #define GET_GLOBALISEL_PREDICATES_DECL
  // #include "PICMidGenGlobalISel.inc"
  // #undef GET_GLOBALISEL_PREDICATES_DECL

  // #define GET_GLOBALISEL_TEMPORARIES_DECL
  // #include "PICMidGenGlobalISel.inc"
  // #undef GET_GLOBALISEL_TEMPORARIES_DECL
};

} // anonymous namespace

// #define GET_GLOBALISEL_IMPL
// #include "PICMidGenGlobalISel.inc"
// #undef GET_GLOBALISEL_IMPL

PICMidInstructionSelector::PICMidInstructionSelector(
    const PICMidTargetMachine &TM, PICMidSubtarget &STI,
    PICMidRegisterBankInfo &RBI)
    : STI(STI), TII(*STI.getInstrInfo()), TRI(*STI.getRegisterInfo()),
      RBI(RBI) {}
// #define GET_GLOBALISEL_PREDICATES_INIT
// #include "PICMidGenGlobalISel.inc"
// #undef GET_GLOBALISEL_PREDICATES_INIT
// #define GET_GLOBALISEL_TEMPORARIES_INIT
// #include "PICMidGenGlobalISel.inc"
// #undef GET_GLOBALISEL_TEMPORARIES_INIT
// {}

bool PICMidInstructionSelector::select(MachineInstr &I) {
  //   if (selectImpl(I, *CoverageInfo)) {
  //     return true;
  //   }
  return false;
}

InstructionSelector *
llvm::createPICMidInstructionSelector(const PICMidTargetMachine &TM,
                                      PICMidSubtarget &STI,
                                      PICMidRegisterBankInfo &RBI) {
  return new PICMidInstructionSelector(TM, STI, RBI);
}
