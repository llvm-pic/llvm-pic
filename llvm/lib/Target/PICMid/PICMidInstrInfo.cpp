#include "PICMidInstrInfo.h"

#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "PICMidFrameLowering.h"

#include "PICMidSubtarget.h"

using namespace llvm;

#define DEBUG_TYPE "picmid-instrinfo"

static bool isTargetCopy(MachineInstr &MI) {
  switch (MI.getOpcode()) {
  case PICMid::G_MOVF_W:
  case PICMid::G_MOVWF:
    return true;
  default:
    return false;
  }
}

static bool isCopyRedundant(MachineIRBuilder &Builder, Register Dst,
                            Register Src) {
  if (Dst == Src)
    return true;
  const TargetRegisterInfo *TRI = Builder.getMRI()->getTargetRegisterInfo();
  MachineInstr *DstKillMI = nullptr;
  for (MachineInstr &MI :
       make_range(MachineBasicBlock::reverse_iterator(Builder.getInsertPt()),
                  Builder.getMBB().rend())) {
    if (MI.killsRegister(Dst, TRI))
      DstKillMI = &MI;
    if (isTargetCopy(MI)) {
      Register CopyDst = MI.getOperand(0).getReg();
      Register CopySrc = MI.getOperand(1).getReg();
      if ((CopyDst == Dst && CopySrc == Src) ||
          (CopyDst == Src && CopySrc == Dst)) {
        MI.clearRegisterDeads(Dst);
        if (DstKillMI)
          DstKillMI->clearRegisterKills(Dst, TRI);
        return true;
      }
    }
    if (MI.modifiesRegister(Dst, TRI))
      return false;
    if (MI.modifiesRegister(Src, TRI))
      return false;
  }
  return false;
}

#define GET_INSTRINFO_CTOR_DTOR
#include "PICMidGenInstrInfo.inc"
#undef GET_INSTRINFO_CTOR_DTOR

llvm::PICMidInstrInfo::PICMidInstrInfo()
    : PICMidGenInstrInfo() //
{}

void llvm::PICMidInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator MI,
                                        const DebugLoc &DL, MCRegister DestReg,
                                        MCRegister SrcReg, bool KillSrc) const {
  MachineIRBuilder Builder(MBB, MI);
  Builder.setDebugLoc(DL);
  copyPhysRegImpl(Builder, DestReg, SrcReg, false, KillSrc);
}

void llvm::PICMidInstrInfo::copyPhysRegImpl(MachineIRBuilder &Builder,
                                            Register DestReg, Register SrcReg,
                                            bool Force, bool KillSrc) const {
  if (!Force && isCopyRedundant(Builder, DestReg, SrcReg)) {
    return;
  }

  const PICMidSubtarget &STI = Builder.getMF().getSubtarget<PICMidSubtarget>();
  const PICMidRegisterInfo &TRI = *STI.getRegisterInfo();
  const auto &IsClass = [&](Register Reg, const TargetRegisterClass &RC) {
    if (Reg.isPhysical() && !RC.contains(Reg))
      return false;
    if (Reg.isVirtual() &&
        !Builder.getMRI()->getRegClass(Reg)->hasSuperClassEq(&RC))
      return false;
    return true;
  };

  const auto &AreClasses = [&](const TargetRegisterClass &Dest,
                               const TargetRegisterClass &Src) {
    return IsClass(DestReg, Dest) && IsClass(SrcReg, Src);
  };

  if (AreClasses(PICMid::ImagRegClass, PICMid::ImagRegClass)) {
    Builder.buildInstr(PICMid::G_MOVWF).addDef(PICMid::R8).addUse(PICMid::W);
    Builder.buildInstr(PICMid::G_MOVF_W).addDef(PICMid::W).addUse(SrcReg);
    Builder.buildInstr(PICMid::G_MOVWF).addDef(DestReg).addUse(PICMid::W);
    Builder.buildInstr(PICMid::G_MOVF_W).addDef(PICMid::W).addUse(PICMid::R8);
    // No need to do anything
    return;
  }

  if (AreClasses(PICMid::WcRegClass, PICMid::ImagRegClass)) {
    Builder.buildInstr(PICMid::G_MOVF_W).addDef(DestReg).addUse(SrcReg);
    return;
  }

  if (AreClasses(PICMid::ImagRegClass, PICMid::WcRegClass)) {
    Builder.buildInstr(PICMid::G_MOVWF).addDef(DestReg).addUse(SrcReg);
    return;
  }

  llvm_unreachable("Unexpected physical register copy.");
}
