#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDREGISTERBANKINFO_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDREGISTERBANKINFO_H_

#include "llvm/CodeGen/RegisterBankInfo.h"

#define GET_REGBANK_DECLARATIONS
#include "PICMidGenRegisterBank.inc"

namespace llvm {
class PICMidGenRegisterBankInfo : public RegisterBankInfo {
protected:
#define GET_TARGET_REGBANK_CLASS
#include "PICMidGenRegisterBank.inc"
};

class PICMidRegisterBankInfo : public PICMidGenRegisterBankInfo {
public:
  const InstructionMapping &
  getInstrMapping(const MachineInstr &MI) const override;

  void applyMappingImpl(const OperandsMapper &OpdMapper) const override;

  const RegisterBank &getRegBankFromRegClass(const TargetRegisterClass &RC,
                                             LLT Ty) const override;
};
} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDREGISTERBANKINFO_H_
