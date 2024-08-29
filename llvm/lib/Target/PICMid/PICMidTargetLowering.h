#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDTARGETLOWERING_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDTARGETLOWERING_H_

#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class PICMidSubtarget;
class PICMidTargetMachine;

class PICMidTargetLowering : public TargetLowering {
public:
  PICMidTargetLowering(const PICMidTargetMachine &TM,
                       const PICMidSubtarget &STI);
};

} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDTARGETLOWERING_H_