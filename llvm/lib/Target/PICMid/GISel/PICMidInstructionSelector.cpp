//===- PICMidInstructionSelector.cpp - Last GlobalISel pass -----*- cpp -*-===//
//
// Part of LLVM-PIC, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains a special GlobalISel pass for performing instruction
// selection of PIC mid-range instructions when given gMIR.
// It largely relies on tablegen-ed code generated from PICMidInstrLogical.td.
//
//===----------------------------------------------------------------------===//
#include "PICMidInstructionSelector.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/GlobalISel/GIMatchTableExecutorImpl.h"
#include "llvm/CodeGen/GlobalISel/GenericMachineInstrs.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelector.h"
#include "llvm/CodeGen/GlobalISel/LoadStoreOpt.h"
#include "llvm/CodeGen/GlobalISel/MIPatternMatch.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include "llvm/CodeGen/GlobalISel/Utils.h"
#include "llvm/CodeGen/LowLevelType.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Register.h"
#include "llvm/CodeGen/RegisterBankInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/ObjectYAML/MachOYAML.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"

#include "MCTargetDesc/PICMidFixupKinds.h"
#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "PICMidInstrInfo.h"
#include "PICMidRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "picmid-isel"

namespace {

static const TargetRegisterClass getRegClassForType(LLT Ty) {
  switch (Ty.getSizeInBits()) {
  default:
    llvm_unreachable("Invalid type size.");
  case 8:
    return PICMid::ImagRegClass;
  }
}

#define GET_GLOBALISEL_PREDICATE_BITSET
#include "PICMidGenGlobalISel.inc"
#undef GET_GLOBALISEL_PREDICATE_BITSET

class PICMidInstructionSelector : public InstructionSelector {
public:
  PICMidInstructionSelector(const PICMidTargetMachine &TM, PICMidSubtarget &STI,
                            PICMidRegisterBankInfo &RBI);
  bool select(MachineInstr &I) override;

  static const char *getName() { return DEBUG_TYPE; }

private:
  const PICMidSubtarget &STI;
  const PICMidInstrInfo &TII;
  const PICMidRegisterInfo &TRI;
  const PICMidRegisterBankInfo &RBI;

  // TODO Frame to adress conversion

  /// tblgen-erated 'select' implementation, used as the initial selector for
  /// the patterns that don't require complex C++.
  bool selectImpl(MachineInstr &I, CodeGenCoverage &CoverageInfo) const;

  bool selectShift(MachineInstr &I) const;
  bool selectLoadStore(MachineInstr &I) const;
  bool selectFrameIndex(MachineInstr &I) const;

  void constrainGeneric(MachineInstr &I) const;
  void constrainOperandRegClass(MachineOperand &RegMO,
                                const TargetRegisterClass &RegClass) const;

#define GET_GLOBALISEL_PREDICATES_DECL
#include "PICMidGenGlobalISel.inc"
#undef GET_GLOBALISEL_PREDICATES_DECL

#define GET_GLOBALISEL_TEMPORARIES_DECL
#include "PICMidGenGlobalISel.inc"
#undef GET_GLOBALISEL_TEMPORARIES_DECL
};

} // anonymous namespace

#define GET_GLOBALISEL_IMPL
#include "PICMidGenGlobalISel.inc"
#undef GET_GLOBALISEL_IMPL

PICMidInstructionSelector::PICMidInstructionSelector(
    const PICMidTargetMachine &TM, PICMidSubtarget &STI,
    PICMidRegisterBankInfo &RBI)
    : STI(STI), TII(*STI.getInstrInfo()), TRI(*STI.getRegisterInfo()), RBI(RBI),
#define GET_GLOBALISEL_PREDICATES_INIT
#include "PICMidGenGlobalISel.inc"
#undef GET_GLOBALISEL_PREDICATES_INIT
#define GET_GLOBALISEL_TEMPORARIES_INIT
#include "PICMidGenGlobalISel.inc"
#undef GET_GLOBALISEL_TEMPORARIES_INIT
{
}

bool PICMidInstructionSelector::select(MachineInstr &I) {
  if (!I.isPreISelOpcode()) {
    constrainGeneric(I);
    return true;
  }
  if (selectImpl(I, *CoverageInfo)) {
    return true;
  }
  switch (I.getOpcode()) {
  case PICMid::G_STORE:
  case PICMid::G_LOAD:
    return selectLoadStore(I);
  case PICMid::G_SHLE:
  case PICMid::G_LSHRE:
    return selectShift(I);
  case PICMid::G_FRAME_INDEX:
    return selectFrameIndex(I);
  // TODO other instructions
  default:
    return true;
  }
}

bool PICMidInstructionSelector::selectFrameIndex(MachineInstr &I) const {
  // TODO
  // do not use a stack, convert to a VReg instead
  MachineIRBuilder Builder(I);
  MachineFunction *MF = I.getMF();
  MachineRegisterInfo *MRI = Builder.getMRI();
  Register stack = I.getOperand(0).getReg();
  return false;
}

bool PICMidInstructionSelector::selectLoadStore(MachineInstr &I) const {
  MachineIRBuilder Builder(I);
  MachineRegisterInfo *MRI = Builder.getMRI();
  auto s8 = LLT::scalar(8);

  auto [Arg1, Arg2] = I.getFirst2Regs();
  unsigned Opcode = -1;
  switch (I.getOpcode()) {
  case PICMid::G_LOAD:
    Opcode = PICMid::G_MOVF_W;
    break;
  case PICMid::G_STORE:
    Opcode = PICMid::G_MOVWF;
    break;
  }
  Builder.buildInstr(Opcode).addUse(Arg1).addUse(Arg2);
  I.eraseFromParent();

  return true;
}

bool PICMidInstructionSelector::selectShift(MachineInstr &I) const {
  auto [Dst, CarryOut, Src, CarryIn] = I.getFirst4Regs();

  unsigned RotateOpcode;
  switch (I.getOpcode()) {
  case PICMid::G_LSHRE:
    RotateOpcode = PICMid::G_RRF_F;
    break;
  case PICMid::G_SHLE:
    RotateOpcode = PICMid::G_RLF_F;
    break;
  }

  MachineIRBuilder Builder(I);

  auto Asl = Builder.buildInstr(RotateOpcode, {Dst, CarryOut}, {Src, CarryIn});

  // return false;

  if (!constrainSelectedInstRegOperands(*Asl, TII, TRI, RBI)) {
    return false;
  }

  I.eraseFromParent();
  return true;
}

void PICMidInstructionSelector::constrainGeneric(MachineInstr &I) const {
  MachineRegisterInfo &MRI = I.getMF()->getRegInfo();

  for (MachineOperand &Op : I.all_defs()) {
    if (Op.getReg().isPhysical() || MRI.getRegClassOrNull(Op.getReg())) {
      continue;
    }

    LLT Ty = MRI.getType(Op.getReg());
    constrainOperandRegClass(Op, getRegClassForType(Ty));
  }
}

void PICMidInstructionSelector::constrainOperandRegClass(
    MachineOperand &RegMO, const TargetRegisterClass &RegClass) const {
  MachineInstr &MI = *RegMO.getParent();
  MachineRegisterInfo &MRI = MI.getMF()->getRegInfo();
  RegMO.setReg(llvm::constrainOperandRegClass(*MF, TRI, MRI, TII, RBI, MI,
                                              RegClass, RegMO));
}

InstructionSelector *
llvm::createPICMidInstructionSelector(const PICMidTargetMachine &TM,
                                      PICMidSubtarget &STI,
                                      PICMidRegisterBankInfo &RBI) {
  return new PICMidInstructionSelector(TM, STI, RBI);
}
