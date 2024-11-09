//===- PICMidRegisterBankInfo.cpp - GlobalISel pass -------------*- cpp -*-===//
//
// Part of LLVM-PIC, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements a special GlobalISel pass for performing register bank
// allocation of PIC mid-range instructions when given gMIR.
//
// Because all registers are of the same type, all registers are put into a
// single register bank "any".
//
//===----------------------------------------------------------------------===//
#include "PICMidRegisterBankInfo.h"

#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterBank.h"

using namespace llvm;

#define GET_TARGET_REGBANK_IMPL
#include "PICMidGenRegisterBank.inc"

const RegisterBankInfo::InstructionMapping &
llvm::PICMidRegisterBankInfo::getInstrMapping(const MachineInstr &MI) const {
  const auto &Mapping = getInstrMappingImpl(MI);
  if (Mapping.isValid()) {
    return Mapping;
  }

  const auto &MRI = MI.getMF()->getRegInfo();
  unsigned NumOperands = MI.getNumOperands();

  SmallVector<const ValueMapping *, 8> ValMappings(NumOperands);
  for (const auto &I : enumerate(MI.operands())) {
    if (!I.value().isReg())
      continue;
    // Only the destination is expected for PHIs.
    if (MI.isPHI() && I.index() == 1) {
      NumOperands = 1;
      break;
    }
    LLT Ty = MRI.getType(I.value().getReg());
    if (!Ty.isValid())
      continue;
    ValMappings[I.index()] =
        &getValueMapping(0, Ty.getSizeInBits(), PICMid::AnyRegBank);
  }
  return getInstructionMapping(/*ID=*/1, /*Cost=*/1,
                               getOperandsMapping(ValMappings), NumOperands);
}

void llvm::PICMidRegisterBankInfo::applyMappingImpl(
    MachineIRBuilder &Builder, const OperandsMapper &OpdMapper) const {
  applyDefaultMapping(OpdMapper);
}

const RegisterBank &llvm::PICMidRegisterBankInfo::getRegBankFromRegClass(
    const TargetRegisterClass &RC, LLT Ty) const {
  return PICMid::AnyRegBank;
}
