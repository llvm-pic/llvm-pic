#ifndef LLVM_LIB_TARGET_PICMID_PICMIDCALLLOWERING_H
#define LLVM_LIB_TARGET_PICMID_PICMIDCALLLOWERING_H

#include "llvm/CodeGen/GlobalISel/CallLowering.h"

namespace llvm {

class PICMidTargetLowering;

class PICMidCallLowering: public CallLowering {
public:
  PICMidCallLowering(const llvm::TargetLowering *TL) : CallLowering(TL) {}

  bool lowerReturn(MachineIRBuilder &MIRBuilder, const Value *Val,
                   ArrayRef<Register> VRegs,
                   FunctionLoweringInfo &FLI) const override;

  bool lowerFormalArguments(MachineIRBuilder &MIRBuilder, const Function &F,
                            ArrayRef<ArrayRef<Register>> VRegs,
                            FunctionLoweringInfo &FLI) const override;

  bool lowerCall(MachineIRBuilder &MIRBuilder,
                 CallLoweringInfo &Info) const override;
  
  bool enableBigEndian() const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_MIPS_MIPSCALLLOWERING_H
