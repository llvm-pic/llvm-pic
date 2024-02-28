#include "PICMidMCInstLower.h"

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

  return false;
}
