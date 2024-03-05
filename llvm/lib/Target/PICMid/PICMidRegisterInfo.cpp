#include "PICMidRegisterInfo.h"
#include "MCTargetDesc/PICMidMCTargetDesc.h"
// #include "PICMid.h"
#include "PICMidFrameLowering.h"
// #include "PICMidInstrCost.h"
#include "PICMidInstrInfo.h"
// #include "PICMidMachineFunctionInfo.h"
#include "PICMidSubtarget.h"

#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/LivePhysRegs.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/CodeGen/VirtRegMap.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/Support/ErrorHandling.h"

#define DEBUG_TYPE "picmid-reginfo"

#define GET_REGINFO_TARGET_DESC
#include "PICMidGenRegisterInfo.inc"

using namespace llvm;

llvm::PICMidRegisterInfo::PICMidRegisterInfo()
    : PICMidGenRegisterInfo(0, 0, 0, 0, 0), Reserved(getNumRegs()) {}

const MCPhysReg *
llvm::PICMidRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  const PICMidFrameLowering &TFI = *getFrameLowering(*MF);
  return TFI.isISR(*MF) ? PICMid_Interrupt_CSR_SaveList : PICMid_CSR_SaveList;
}

BitVector
llvm::PICMidRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  return this->Reserved;
}

bool llvm::PICMidRegisterInfo::eliminateFrameIndex(
    MachineBasicBlock::iterator MI, int SPAdj, unsigned FIOperandNum,
    RegScavenger *RS) const {
  return false;
}

Register
llvm::PICMidRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return Register();
}

uint16_t llvm::PICMidRegisterInfo::getHwEncoding(Register Reg) const {
  return PICMidRegEncodingTable[Reg.id()];
}
