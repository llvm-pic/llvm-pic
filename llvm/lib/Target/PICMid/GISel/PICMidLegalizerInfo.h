#ifndef LLVM_LIB_TARGET_PICMid_PICMidMACHINELEGALIZER_H
#define LLVM_LIB_TARGET_PICMid_PICMidMACHINELEGALIZER_H

#include "llvm/CodeGen/GlobalISel/LegalizerInfo.h"
#include <llvm/CodeGen/GlobalISel/Legalizer.h>

namespace llvm {

class PICMidSubtarget;

class PICMidLegalizerInfo : public LegalizerInfo {
public:
  PICMidLegalizerInfo();
  bool legalizeCustom(LegalizerHelper &Helper, MachineInstr &MI) const override;
  bool legalizeIntrinsic(LegalizerHelper &Helper,
                         MachineInstr &MI) const override;
};

} // namespace llvm
#endif