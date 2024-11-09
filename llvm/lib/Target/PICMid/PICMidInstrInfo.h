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

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
                   const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const override;

private:
  void copyPhysRegImpl(MachineIRBuilder &Builder, Register DestReg,
                       Register SrcReg, bool Force = false,
                       bool KillSrc = false) const;
};
} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDINSTRINFO_H_