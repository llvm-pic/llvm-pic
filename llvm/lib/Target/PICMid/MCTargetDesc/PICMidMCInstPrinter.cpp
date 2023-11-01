#include "PICMidMCInstPrinter.h"
#include "MCTargetDesc/PICMidMCExpr.h"

namespace llvm {

void PICMidMCInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                    StringRef Annot, const MCSubtargetInfo &STI,
                                    raw_ostream &OS) {
  assert(getMnemonic(MI).second && "Missing opcode for instruction");

  std::string airyOperands;
  raw_string_ostream airyOperandStream(airyOperands);
  printInstruction(MI, Address, airyOperandStream);

  size_t spacesSeen = 0;
  std::string correctOperands;

  for (const auto &letter : airyOperands) {
    if (isspace(letter) == 0) {
      correctOperands += letter;
      continue;
    }

    if (++spacesSeen <= 2) {
      correctOperands += '\t';
    }
  }

  OS << correctOperands;
}

void PICMidMCInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                       raw_ostream &O) {
  const MCOperand &op = MI->getOperand(OpNo);

  if (op.isReg()) {
    printRegName(O, op.getReg());
  } else if (op.isImm()) {
    O << formatImm(op.getImm());
  } else if (op.isExpr()) {
    // if (const auto *mme = dynamic_cast<PICMidMCExpr>(op.getExpr())) { //
    // TODO: Implement
    // }
    op.getExpr()->print(O, &MAI);
  } else {
    assert(0 && "Unknown operand kind in printOperand");
  }
}

void PICMidMCInstPrinter::printBranchOperand(const MCInst *MI, unsigned OpNo,
                                             raw_ostream &O) {
  const MCOperand &op = MI->getOperand(OpNo);
  if (!op.isImm()) {
    return printOperand(MI, OpNo, O);
  }

  uint64_t target = op.getImm();
  O << formatImm(target);
}

void PICMidMCInstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) const {
    OS << getRegisterName(Reg);
}

// void PICMidMCInstPrinter::printCustomAliasOperand(const MCInst *MI,
//                                                   uint64_t Address,
//                                                   unsigned OpIdx,
//                                                   unsigned PrintMethodIdx,
//                                                   raw_ostream &OS) {}
format_object<int64_t> PICMidMCInstPrinter::formatHex(int64_t Value) const {
  switch (PrintHexStyle) {
  case HexStyle::C:
    if (Value < 0) {
      return format("-$%" PRIx64, -Value);
    } else {
      return format("$%" PRIx64, Value);
    }
  case HexStyle::Asm:
    if (Value < 0) {
      return format("-$%" PRIx64, -Value);
    } else {
      return format("$%" PRIx64, Value);
    }
  }
  llvm_unreachable("unsupported print style");
}

format_object<uint64_t> PICMidMCInstPrinter::formatHex(uint64_t Value) const {
  switch (PrintHexStyle) {
  case HexStyle::C:
    return format("$%" PRIx64, Value);
  case HexStyle::Asm:
    return format("$%" PRIx64, Value);
  }
  llvm_unreachable("unsupported print style");
}

#define PRINT_ALIAS_INSTR
#include "PICMidGenAsmWriter.inc"

} // namespace llvm
