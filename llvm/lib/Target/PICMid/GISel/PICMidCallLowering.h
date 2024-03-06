#ifndef LLVM_LIB_TARGET_PICMID_PICMIDCALLLOWERING_H
#define LLVM_LIB_TARGET_PICMID_PICMIDCALLLOWERING_H

#include "llvm/CodeGen/GlobalISel/CallLowering.h"

namespace llvm {

class PICMidCallLowering: public CallLowering {
public:
  PICMidCallLowering(const PICMidCallLowering&TLI);

  bool lowerReturn(MachineIRBuilder &MIRBuilder, const Value *Val,
                   ArrayRef<Register> VRegs,
                   FunctionLoweringInfo &FLI) const override;

  bool lowerFormalArguments(MachineIRBuilder &MIRBuilder, const Function &F,
                            ArrayRef<ArrayRef<Register>> VRegs,
                            FunctionLoweringInfo &FLI) const override;

  bool lowerCall(MachineIRBuilder &MIRBuilder,
                 CallLoweringInfo &Info) const override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_PICMID_PICMIDCALLLOWERING_H
