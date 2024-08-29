#include "PICMidFrameLowering.h"

using namespace llvm;

#define DEBUG_TYPE "picmid-framelowering"

llvm::PICMidFrameLowering::PICMidFrameLowering()
    : TargetFrameLowering(StackGrowsDown, Align(1), 0) {}

void llvm::PICMidFrameLowering::emitPrologue(MachineFunction &MF,
                                             MachineBasicBlock &MBB) const {}

void llvm::PICMidFrameLowering::emitEpilogue(MachineFunction &MF,
                                             MachineBasicBlock &MBB) const {}

bool llvm::PICMidFrameLowering::hasFP(const MachineFunction &MF) const {
  return false;
}

bool llvm::PICMidFrameLowering::isISR(const MachineFunction &MF) const {
  const Function &F = MF.getFunction();
  if (F.hasFnAttribute("no-isr"))
    return false;
  return F.hasFnAttribute("interrupt") ||
         F.hasFnAttribute("interrupt-norecurse");
}
