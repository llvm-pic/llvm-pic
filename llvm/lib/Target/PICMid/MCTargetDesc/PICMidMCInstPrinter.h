#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCINSTRPRINTER_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCINSTRPRINTER_H_

#include <utility>

#include "llvm/MC/MCInstPrinter.h"

#include "MCTargetDesc/PICMidMCTargetDesc.h"

namespace llvm {
class PICMidMCInstPrinter : public MCInstPrinter {
public:
  PICMidMCInstPrinter(const MCAsmInfo &mai, const MCInstrInfo &mii,
                      const MCRegisterInfo &mri)
      : MCInstPrinter(mai, mii, mri) {}

  void printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
                 const MCSubtargetInfo &STI, raw_ostream &OS) override;
  void printRegName(raw_ostream &OS, MCRegister Reg) const override;

  // generated by TableGen
  bool printAliasInstr(const MCInst *MI, uint64_t Address, raw_ostream &OS);
  void printInstruction(const MCInst *MI, uint64_t Address, raw_ostream &O);
  std::pair<const char *, uint64_t> getMnemonic(const MCInst *MI) override;

  void printOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);
  void printBranchOperand(const MCInst *MI,
                          /* uint64_t Address,*/ unsigned OpNo, raw_ostream &O);
  void printCustomAliasOperand(const MCInst *MI, uint64_t Address,
                               unsigned OpIdx, unsigned PrintMethodIdx,
                               raw_ostream &OS);
  static const char *getRegisterName(MCRegister Reg);
//   static const char *getRegisterName(MCRegister Reg, unsigned AltIdx);

  /// Utility function to print immediates in decimal or hex.
  format_object<int64_t> formatImm(int64_t Value) const {
    return PrintImmHex ? formatHex(Value) : formatDec(Value);
  }

  /// Utility functions to print decimal/hexadecimal values.
  format_object<int64_t> formatHex(int64_t Value) const;
  format_object<uint64_t> formatHex(uint64_t Value) const;
};
} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCINSTRPRINTER_H_