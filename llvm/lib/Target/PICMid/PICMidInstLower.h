#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDMCINSTLOWER_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDMCINSTLOWER_H_

#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCContext.h"

namespace llvm {
class PICMidMCInstLower {
  MCContext &Ctx;
  const AsmPrinter &AP;

public:
  PICMidMCInstLower(MCContext &Ctx, const AsmPrinter &AP) : Ctx(Ctx), AP(AP) {}

  void lower(const MachineInstr *MI, MCInst &OutMI);
  bool lowerOperand(const MachineOperand &MO, MCOperand &MCOp);

private:
  MCOperand lowerSymbolOperand(const MachineOperand &MO, const MCSymbol *Sym);
};
} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDMCINSTLOWER_H_
