#include "PICMidInstLower.h"
#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "PICMidFunctionInfo.h"
#include "PICMidRegisterInfo.h"
#include "PICMidSubtarget.h"

#define DEBUG_TYPE "picmid-mcinstlower"

using namespace llvm;

void llvm::PICMidMCInstLower::lower(const MachineInstr *MI, MCInst &OutMI) {
  switch (MI->getOpcode()) {
    //   case /* constant-expression */:
    //     /* code */
    //     break;

  default:
    OutMI.setOpcode(MI->getOpcode());
    break;
  }
}

bool llvm::PICMidMCInstLower::lowerOperand(const MachineOperand &MO,
                                           MCOperand &MCOp) {
  const auto &FuncInfo =
      *MO.getParent()->getMF()->getInfo<PICMidFunctionInfo>();
  const PICMidRegisterInfo &TRI = *MO.getParent()
                                       ->getMF()
                                       ->getSubtarget<PICMidSubtarget>()
                                       .getRegisterInfo();
  auto GetTotal = [&]() {
    size_t Idx = &MO - MO.getParent()->operands_begin();
    switch (MO.getParent()->getDesc().operands()[Idx].OperandType) {
    default:
      llvm_unreachable("Unexpected operand type.");
    case PICMidOp::OPERAND_IMM1:
      return 1 << 1;
    case PICMidOp::OPERAND_IMM3:
      return 1 << 3;
    case PICMidOp::OPERAND_ADDR7:
      return 1 << 7;
    case PICMidOp::OPERAND_IMM8:
      return 1 << 8;
    case PICMidOp::OPERAND_PCABS11:
      return 1 << 11;
    }
  };

  switch (MO.getType()) {
  default:
    LLVM_DEBUG(dbgs() << "Operand: " << MO << "\n");
    report_fatal_error("Operand type not implemented.");
    return false;
  case MachineOperand::MO_RegisterMask:
    return false;
  case MachineOperand::MO_BlockAddress:
    MCOp =
        lowerSymbolOperand(MO, AP.GetBlockAddressSymbol(MO.getBlockAddress()));
    break;
  case MachineOperand::MO_ExternalSymbol:
    MCOp =
        lowerSymbolOperand(MO, AP.GetExternalSymbolSymbol(MO.getSymbolName()));
    break;
  case MachineOperand::MO_Immediate:
    MCOp = MCOperand::createImm(MO.getImm() >= 0 ? MO.getImm()
                                                 : MO.getImm() + GetTotal());
    break;
  case MachineOperand::MO_Register:
    if (MO.isImplicit() || !PICMid::ImagRegClass.contains(MO.getReg())) {
      return false;
    }

    MCOp = MCOperand::createImm(TRI.getHwEncoding(MO.getReg()));
  }

  return true;
}

MCOperand llvm::PICMidMCInstLower::lowerSymbolOperand(const MachineOperand &MO,
                                                      const MCSymbol *Sym) {
  const MachineFrameInfo &MFI = MO.getParent()->getMF()->getFrameInfo();

  const MCExpr *Expr = MCSymbolRefExpr::create(Sym, Ctx);
  return MCOperand::createExpr(Expr);
}
