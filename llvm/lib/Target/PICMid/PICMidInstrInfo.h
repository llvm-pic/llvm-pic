#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDINSTRINFO_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDINSTRINFO_H_

#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "PICMidGenInstrInfo.inc"

namespace llvm {
class PICMidInstrInfo : public PICMidGenInstrInfo {
public:
  PICMidInstrInfo();
};
} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDINSTRINFO_H_