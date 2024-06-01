#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDFRAMELOWERING_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDFRAMELOWERING_H_

#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include <cstdint>

namespace llvm {
class PICMidFrameLowering : public TargetFrameLowering {
public:
  PICMidFrameLowering();

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  bool hasFP(const MachineFunction &MF) const override;
  bool isISR(const MachineFunction &MF) const;
};
} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDFRAMELOWERING_H_