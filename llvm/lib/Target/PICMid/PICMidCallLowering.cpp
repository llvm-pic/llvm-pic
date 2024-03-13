#include "PICMidCallLowering.h"
#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include <cstdio>

using namespace llvm;

  bool PICMidCallLowering::lowerReturn(MachineIRBuilder &MIRBuilder, const Value *Val,
                   ArrayRef<Register> VRegs,
                   FunctionLoweringInfo &FLI) const {
									//MIRBuilder.insertInstr(PICMid::RETURN);
									printf("lowerReturn");
								return true;	
									 }

  bool PICMidCallLowering::lowerFormalArguments(MachineIRBuilder &MIRBuilder, const Function &F,
                            ArrayRef<ArrayRef<Register>> VRegs,
                            FunctionLoweringInfo &FLI) const {
									printf("lowerFormal");
									//MIRBuilder.insertInstr(PICMid::RETURN);
								return true;	
														}

  bool PICMidCallLowering::lowerCall(MachineIRBuilder &MIRBuilder,
                 CallLoweringInfo &Info) const {
									printf("lowerCall");
									//MIRBuilder.insertInstr(PICMid::RETURN);
								return true;	
								 }
