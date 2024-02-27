#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDINSTRUCTIONSELECTOR_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDINSTRUCTIONSELECTOR_H_

// #include "PICMidRegisterBankInfo.h"
#include "PICMidRegisterBankInfo.h"
#include "PICMidSubtarget.h"
#include "PICMidTargetMachine.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelector.h"

namespace llvm {
InstructionSelector *
createPICMidInstructionSelector(const PICMidTargetMachine &TM,
                                PICMidSubtarget &STI,
                                PICMidRegisterBankInfo &RBI);
} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDINSTRUCTIONSELECTOR_H_