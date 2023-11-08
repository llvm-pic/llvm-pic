//===-- PICMidMCTargetDesc.h - MOS Target Descriptions ----------*- C++ -*-===//
//
// Part of LLVM-PIC, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides PICMid specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_PICMID_MCTARGET_DESC_H
#define LLVM_PICMID_MCTARGET_DESC_H

#include <memory>

#include "llvm/ADT/Sequence.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/DataTypes.h"

#include "TargetInfo/PICMidTargetInfo.h"

namespace llvm {
MCInstrInfo *createPICMidMCInstrInfo();
MCAsmBackend *createPICMidMCAsmBackend(const Target &t,
                                       const MCSubtargetInfo &sti,
                                       const MCRegisterInfo &mri,
                                       const MCTargetOptions &to);
std::unique_ptr<MCObjectTargetWriter>
createPICMidELFObjectWriter(uint8_t osabi);
} // namespace llvm

#define GET_REGINFO_ENUM
#include "PICMidGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "PICMidGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "PICMidGenSubtargetInfo.inc"

namespace llvm {
template <> struct enum_iteration_traits<decltype(PICMid::NoRegister)> {
  static constexpr bool is_iterable = true;
};

namespace PICMidOp {

enum OperandType : unsigned {
    OPERAND_IMM8 = MCOI::OPERAND_FIRST_TARGET,
    OPERAND_IMM1,
    OPERAND_IMM3,
    OPERAND_PCABS11,
    OPERAND_ADDR7,
};

} // namespace PICMidOp

} // namespace llvm

#endif // LLVM_PICMID_MCTARGET_DESC_H
