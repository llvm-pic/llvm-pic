#include "PICMidCallLowering.h"
#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "llvm/CodeGen/FunctionLoweringInfo.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"

using namespace llvm;

bool PICMidCallLowering::lowerReturn(MachineIRBuilder &MIRBuilder,
                                     const Value *Val, ArrayRef<Register> VRegs,
                                     FunctionLoweringInfo &FLI) const {
	auto retInstr = MIRBuilder.buildInstrNoInsert(PICMid::RETURN);
	if (!VRegs.empty()) {
		// this might fail on more complex data structures
		Type *T = Val->getType();
    if (T->isPointerTy()) {
      // optionally use RETLW, however this purely safes memory space, and should be done in the combiner
    } else if (T->isStructTy()) {
      // TODO
      return false;
    }
	} 
  MIRBuilder.insertInstr(retInstr);
  return true;
}

bool PICMidCallLowering::lowerFormalArguments(
    MachineIRBuilder &MIRBuilder, const Function &F,
    ArrayRef<ArrayRef<Register>> VRegs, FunctionLoweringInfo &FLI) const {
  // MIRBuilder.insertInstr(PICMid::RETURN);
  // TODO
  return true;
}

bool PICMidCallLowering::lowerCall(MachineIRBuilder &MIRBuilder,
                                   CallLoweringInfo &Info) const {
  MIRBuilder.buildInstr(PICMid::CALL);
  return true;
}

bool PICMidCallLowering::enableBigEndian() const { return true; }