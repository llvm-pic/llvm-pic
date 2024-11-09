#ifndef LLVM_LIB_TARGET_PICMid_PICMidMACHINELEGALIZER_H
#define LLVM_LIB_TARGET_PICMid_PICMidMACHINELEGALIZER_H

#include "llvm/CodeGen/GlobalISel/Legalizer.h"
#include "llvm/CodeGen/GlobalISel/LegalizerHelper.h"
#include "llvm/CodeGen/GlobalISel/LegalizerInfo.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfoImpls.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

namespace llvm {

class PICMidLegalizerInfo : public LegalizerInfo {
  LLT S1 = LLT::scalar(1);
  LLT S8 = LLT::scalar(8);
  LLT P = LLT::pointer(0, 8);

public:
  PICMidLegalizerInfo();
  bool legalizeCustom(LegalizerHelper &Helper, MachineInstr &MI,
                      LostDebugLocObserver &LocObserver) const override;
  bool legalizeBrCond(LegalizerHelper &Helper, MachineInstr &MI,
                      MachineRegisterInfo &MRI) const;
  bool legalizeICmp(LegalizerHelper &Helper, MachineInstr &MI,
                    MachineRegisterInfo &MRI) const;
  bool legalizeSelect(LegalizerHelper &Helper, MachineInstr &MI) const;
};

} // namespace llvm
#endif