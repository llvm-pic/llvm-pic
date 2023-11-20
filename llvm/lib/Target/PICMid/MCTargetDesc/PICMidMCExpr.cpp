#include "PICMidMCExpr.h"

#include "llvm/MC/MCAsmLayout.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCValue.h"

namespace llvm {

namespace {

const struct ModifierEntry {
  const char *const Spelling;
  PICMidMCExpr::VariantKind VariantKind;
} ModifierNames[] = {
    {"picmid1", PICMidMCExpr::VK_PICMID_IMM1},
    {"picmid3", PICMidMCExpr::VK_PICMID_IMM3},
    {"picmid8", PICMidMCExpr::VK_PICMID_IMM8},
    {"picmid7", PICMidMCExpr::VK_PICMID_ADDR7},
    {"picmid11", PICMidMCExpr::VK_PICMID_PCABS11},
};

} // end of anonymous namespace

const PICMidMCExpr *PICMidMCExpr::create(VariantKind Kind, const MCExpr *Expr,
                                         bool IsNegated, MCContext &Ctx) {
  return new (Ctx) PICMidMCExpr(Kind, Expr, IsNegated);
}

PICMid::Fixups PICMidMCExpr::getFixupKind() const {
  switch (Kind) {
  case VK_PICMID_IMM1:
    return PICMid::Imm1;
  case VK_PICMID_IMM3:
    return PICMid::Imm3;
  case VK_PICMID_IMM8:
    return PICMid::Imm8;
  case VK_PICMID_ADDR7:
    return PICMid::Addr7;
  case VK_PICMID_PCABS11:
    return PICMid::PCAbs11;
  case VK_PICMID_NONE:
    llvm_unreachable("Uninitialized expression.");
  }

  return PICMid::LastTargetFixupKind;
}

const char *PICMidMCExpr::getName() const {
  const auto &Modifier = std::find_if(
      std::begin(ModifierNames), std::end(ModifierNames),
      [this](const ModifierEntry &Mod) { return Mod.VariantKind == Kind; });

  if (Modifier == std::end(ModifierNames)) {
    return nullptr;
  }

  return Modifier->Spelling;
}

bool PICMidMCExpr::evaluateAsConstant(int64_t &Result) const {
  MCValue Value;
  bool IsRelocatable =
      getSubExpr()->evaluateAsRelocatable(Value, nullptr, nullptr);

  if (!IsRelocatable ||
      !Value.isAbsolute()) { // TODO: Investigate why it has to be a relocatable
    return false;
  }

  Result = evaluateAsInt64(Value.getConstant());
  return true;
}

void PICMidMCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {
  assert(Kind != VK_PICMID_NONE);
  if (isNegated()) {
    OS << '-';
  }

//   OS << getName() << '(';
  getSubExpr()->print(OS, MAI);
//   OS << ')';
}

bool PICMidMCExpr::evaluateAsRelocatableImpl(MCValue &Result,
                                             const MCAsmLayout *Layout,
                                             const MCFixup *Fixup) const {
  MCValue Value;
  bool IsRelocatable = SubExpr->evaluateAsRelocatable(Value, Layout, Fixup);

  if (!IsRelocatable) {
    return false;
  }

  if (Value.isAbsolute()) {
    Result = MCValue::get(evaluateAsInt64(Value.getConstant()));
  } else {
    if (!Layout) {
      return false;
    }

    const MCSymbolRefExpr *Sym = Value.getSymA();
    MCSymbolRefExpr::VariantKind Modifier = Sym->getKind();
    if (Modifier != MCSymbolRefExpr::VK_None) {
      return false;
    }

    MCContext &Context = Layout->getAssembler().getContext();
    Sym = MCSymbolRefExpr::create(&Sym->getSymbol(), Modifier, Context);
    Result = MCValue::get(Sym, Value.getSymB(), Value.getConstant());
  }

  return true;
}

void PICMidMCExpr::visitUsedExpr(MCStreamer &Streamer) const {
  Streamer.visitUsedExpr(*getSubExpr());
}

PICMidMCExpr::VariantKind PICMidMCExpr::getKindByName(StringRef Name) {
  const auto &Modifier = std::find_if(
      std::begin(ModifierNames), std::end(ModifierNames),
      [&Name](const ModifierEntry &Mod) { return Mod.Spelling == Name; });

  if (Modifier == std::end(ModifierNames)) {
    return VK_PICMID_NONE;
  }

  return Modifier->VariantKind;
}

int64_t PICMidMCExpr::evaluateAsInt64(int64_t Value) const {
  int64_t out = Value;
  if (isNegated()) {
    out = -out;
  }

  switch (Kind) {
  case VK_PICMID_IMM1:
    out &= 0b1;
    if (isNegated()) {
      out ^= 0b1;
    }
    break;
  case VK_PICMID_IMM3:
    out &= 0b111;
    break;
  case VK_PICMID_ADDR7:
    out &= 0x7F;
    break;
  case VK_PICMID_IMM8:
    out &= 0xFF;
    break;
  case VK_PICMID_PCABS11:
    out &= 0x7FF;
    break;
  case VK_PICMID_NONE:
    llvm_unreachable("Uninitialized expression.");
  }

  return static_cast<uint64_t>(out);
}

} // namespace llvm
