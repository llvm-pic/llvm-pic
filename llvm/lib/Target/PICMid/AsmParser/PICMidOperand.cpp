#include "PICMidOperand.h"

#include <memory>

using namespace llvm;

PICMidOperand::PICMidOperand(const PICMidSubtarget &sti, const MCExpr *val,
                             SMLoc s, SMLoc e)
    : Kind(Immediate), start(s), end(e), imm(val), sti(sti) {}

PICMidOperand::PICMidOperand(const PICMidSubtarget &sti, unsigned regNum,
                             SMLoc s, SMLoc e)
    : Kind(Register), start(s), end(e), reg(regNum), sti(sti) {}

PICMidOperand::PICMidOperand(const PICMidSubtarget &sti, StringRef str, SMLoc s)
    : Kind(Token), start(s), tok(str), sti(sti) {}

bool PICMidOperand::is(KindTy K) const { return (Kind == K); }

bool PICMidOperand::isToken() const { return is(Token); }

bool PICMidOperand::isReg() const { return is(Register); }

bool PICMidOperand::isImm() const { return is(Immediate); }

bool PICMidOperand::isMem() const {
  assert(false);
  return false;
}

SMLoc PICMidOperand::getStartLoc() const { return start; }

SMLoc PICMidOperand::getEndLoc() const { return end; }

const StringRef &PICMidOperand::getToken() const {
  assert(isToken());
  return tok;
}

const MCExpr *PICMidOperand::getImm() const {
  assert(isImm());
  return imm;
}

unsigned PICMidOperand::getReg() const {
  assert(isReg());
  return reg;
}

bool PICMidOperand::isImm1() const { return isImmediate<0, 1>(); }

bool PICMidOperand::isImm3() const { return isImmediate<0, 0b111>(); }

bool PICMidOperand::isImm8() const { return isImmediate<0, 0xFF>(); }

bool PICMidOperand::isPCAbs11() const {
  return isImmediate<0, 0b1111'1111'111>();
}

bool PICMidOperand::isAddr7() const { return isImmediate<0, 0b1111'111>(); }

void PICMidOperand::addExpr(MCInst &inst, const MCExpr *expr) {
  if (expr == 0) {
    inst.addOperand(MCOperand::createImm(0));
  } else if (const auto *ce = dyn_cast<MCConstantExpr>(expr)) {
    inst.addOperand(MCOperand::createImm(ce->getValue()));
  } else {
    inst.addOperand(MCOperand::createExpr(expr));
  }
}

void PICMidOperand::addImmOperands(MCInst &inst, unsigned n) const {
  assert(isImm() && "Unexpected operand kind");
  assert(n == 1 && "Invalid number of operands, has to be exactly one");

  const MCExpr *expr = getImm();
  addExpr(inst, expr);
}

void PICMidOperand::addRegOperands(MCInst &inst, unsigned n) const {
  inst.addOperand(MCOperand::createReg(getReg()));
}

void PICMidOperand::addImm1Operands(MCInst &inst, unsigned n) const {
  addImmOperands(inst, n);
}

void PICMidOperand::addImm3Operands(MCInst &inst, unsigned n) const {
  addImmOperands(inst, n);
}

void PICMidOperand::addImm8Operands(MCInst &inst, unsigned n) const {
  addImmOperands(inst, n);
}

void PICMidOperand::addPCAbs11Operands(MCInst &inst, unsigned n) const {
  addImmOperands(inst, n);
}

void PICMidOperand::addAddr7Operands(MCInst &inst, unsigned n) const {
  addImmOperands(inst, n);
}

std::unique_ptr<PICMidOperand>
PICMidOperand::createImm(const PICMidSubtarget &sti, const MCExpr *val, SMLoc s,
                         SMLoc e) {
  return std::make_unique<PICMidOperand>(sti, val, s, e);
}
std::unique_ptr<PICMidOperand>
PICMidOperand::createReg(const PICMidSubtarget &sti, unsigned regNum, SMLoc s,
                         SMLoc e) {
  return std::make_unique<PICMidOperand>(sti, regNum, s, e);
}
std::unique_ptr<PICMidOperand>
PICMidOperand::createToken(const PICMidSubtarget &sti, StringRef str, SMLoc s) {
  return std::make_unique<PICMidOperand>(sti, str, s);
}

void PICMidOperand::print(raw_ostream &o) const {
  switch (Kind) {
  case None:
    o << "None";
    break;
  case Token:
    o << "Token: \"" << getToken() << "\"";
    break;
  case Register:
    o << "Register: \"" << getReg() << "\"";
    break;
  case Immediate:
    o << "Immediate: \"" << *getImm() << "\"";
    break;
    // default: // Already covered
    //     assert(false && "Unrecognized PICMidOperand kind!");
    //     return;
  }

  o << "\n";
}
