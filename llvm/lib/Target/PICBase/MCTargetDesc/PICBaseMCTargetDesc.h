#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICBASE_MCTARGETDESC_PICBASEMCTARGETDESC_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICBASE_MCTARGETDESC_PICBASEMCTARGETDESC_H_

#include "llvm/ADT/Sequence.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/DataTypes.h"

namespace llvm {

MCInstrInfo *createPICBaseMCInstrInfo();

} // namespace llvm

#define GET_REGINFO_ENUM
#include "PICBaseGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "PICBaseGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "PICBaseGenSubtargetInfo.inc"

namespace llvm {

namespace PICBaseOp {

enum OperandType : unsigned {
  OPERAND_IMM8 = MCOI::OPERAND_FIRST_TARGET,
  OPERAND_IMM1,
  OPERAND_IMM3,
  OPERAND_PCABS8,
  OPERAND_PCABS9,
  OPERAND_ADDR3,
  OPERAND_ADDR5,
};

}

} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICBASE_MCTARGETDESC_PICBASEMCTARGETDESC_H_