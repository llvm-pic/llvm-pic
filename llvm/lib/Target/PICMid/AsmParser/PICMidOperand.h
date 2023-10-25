#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_ASMPARSER_PICMIDOPERAND_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_ASMPARSER_PICMIDOPERAND_H_

#include "PICMidSubtarget.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCParser/MCParsedAsmOperand.h"

namespace llvm {

#define GET_REGISTER_MATCHER
#include "PICMidGenAsmMatcher.inc"

class PICMidOperand : public MCParsedAsmOperand {
public:
    PICMidOperand() = delete;
    PICMidOperand(const PICMidSubtarget & sti, const MCExpr *val, SMLoc s, SMLoc e);
    PICMidOperand(const PICMidSubtarget & sti, unsigned regNum, SMLoc s, SMLoc e);
    PICMidOperand(const PICMidSubtarget & sti, StringRef str, SMLoc s);
private:
  enum KindTy { None, Token, Register, Immediate } Kind{None};

  SMLoc start, end;
  StringRef tok;
  const MCExpr *imm{};
  unsigned int reg{};
  const PICMidSubtarget & sti;

public:
  template <int64_t Low, int64_t High> bool isImmediate() const;

  bool is(KindTy K) const;
  bool isToken() const override;
  bool isReg() const override;
  bool isImm() const override;
  bool isMem() const override;

  SMLoc getStartLoc() const override;
  SMLoc getEndLoc() const override;

  const StringRef &getToken() const;
  const MCExpr *getImm() const;
  unsigned getReg() const override;

  bool isImm1() const;
  bool isImm3() const;
  bool isImm8() const;
  bool isPCAbs11() const;
  bool isAddr7() const;

  static void addExpr(MCInst &inst, const MCExpr *expr);
  void addImmOperands(MCInst &inst, unsigned n) const;
  void addRegOperands(MCInst &inst, unsigned n) const;
  void addImm1Operands(MCInst &inst, unsigned n) const;
  void addImm3Operands(MCInst &inst, unsigned n) const;
  void addImm8Operands(MCInst &inst, unsigned n) const;
  void addPCAbs11Operands(MCInst &inst, unsigned n) const;
  void addAddr7Operands(MCInst &inst, unsigned n) const;

  static std::unique_ptr<PICMidOperand>
  createImm(const PICMidSubtarget &sti, const MCExpr *val, SMLoc s, SMLoc e);
  static std::unique_ptr<PICMidOperand>
  createReg(const PICMidSubtarget &sti, unsigned regNum, SMLoc s, SMLoc e);
  static std::unique_ptr<PICMidOperand> createToken(const PICMidSubtarget &sti,
                                                    StringRef str, SMLoc s);

  void print(raw_ostream &o) const override;
};

template <int64_t Low, int64_t High>
inline bool PICMidOperand::isImmediate() const {
  if (!isImm()) { return false; }

// TODO: Handle PIC specific modifiers

  const auto * sre = dyn_cast<MCSymbolRefExpr>(getImm());
  if (sre) { return true; }

  const auto *ce = dyn_cast<MCConstantExpr>(getImm());
  if (!ce) { return true; }

  int64_t value = ce->getValue();
  return value >= Low && value <= High;
}

} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_ASMPARSER_PICMIDOPERAND_H_