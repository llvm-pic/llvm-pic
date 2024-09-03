//===- PICMidRegisterBankInfo.h - GlobalISel pass --------------*- cpp -*-===//
//
// Part of LLVM-PIC, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares a special GlobalISel pass for performing register bank
// allocation of PIC mid-range instructions when given gMIR.
//
// Because all registers are of the same type, all registers are put into a
// single register bank "any".
//
//===----------------------------------------------------------------------===//
#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDREGISTERBANKINFO_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDREGISTERBANKINFO_H_

#include "llvm/CodeGen/RegisterBankInfo.h"

#define GET_REGBANK_DECLARATIONS
#include "PICMidGenRegisterBank.inc"

namespace llvm {
class PICMidGenRegisterBankInfo : public RegisterBankInfo {
protected:
#define GET_TARGET_REGBANK_CLASS
#include "PICMidGenRegisterBank.inc"
};

class PICMidRegisterBankInfo : public PICMidGenRegisterBankInfo {
public:
  const InstructionMapping &
  getInstrMapping(const MachineInstr &MI) const override;

  void applyMappingImpl(MachineIRBuilder &Builder,
                        const OperandsMapper &OpdMapper) const override;

  const RegisterBank &getRegBankFromRegClass(const TargetRegisterClass &RC,
                                             LLT Ty) const override;
};
} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDREGISTERBANKINFO_H_
