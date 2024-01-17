#include "PICMidMCAsmBackend.h"
#include "PICMidELFObjectWriter.h"
#include "PICMidFixupKinds.h"
#include "llvm/MC/MCAsmLayout.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCValue.h"

#include <cassert>

namespace llvm {

PICMidMCAsmBackend::PICMidMCAsmBackend(Triple::OSType OSType)
    : MCAsmBackend(support::little), OSType(OSType) {}

std::unique_ptr<MCObjectTargetWriter>
PICMidMCAsmBackend::createObjectTargetWriter() const {
  return std::make_unique<PICMidELFObjectWriter>(OSType);
}

unsigned PICMidMCAsmBackend::getNumFixupKinds() const {
  return PICMid::NumTargetFixupKinds;
}

const MCFixupKindInfo &
PICMidMCAsmBackend::getFixupKindInfo(MCFixupKind Kind) const {
  if (Kind < FirstTargetFixupKind) {
    return MCAsmBackend::getFixupKindInfo(Kind);
  }
  return PICMidFixupKinds::getFixupKindInfo(static_cast<PICMid::Fixups>(Kind),
                                            this);
}

void PICMidMCAsmBackend::applyFixup(const MCAssembler &Asm,
                                    const MCFixup &Fixup, const MCValue &Target,
                                    MutableArrayRef<char> Data, uint64_t Value,
                                    bool IsResolved,
                                    const MCSubtargetInfo *STI) const {
  unsigned int Kind = Fixup.getKind();
  unsigned int Bits = 0;
  switch (Kind) {
  case PICMid::Imm1:
    Bits = 1;
    break;
  case PICMid::Imm3:
    Bits = 3;
    break;
  case PICMid::Imm8:
    Bits = 8;
    break;
  case PICMid::Addr7:
    Bits = 7;
    break;
  case PICMid::PCAbs11:
    Bits = 11;
    break;
  default:
    llvm_unreachable("unknown fixup kind");
    return;
  }
  unsigned int Bytes = Bits / 8;

  uint32_t Offset = Fixup.getOffset();

  assert(((Bytes + Offset) <= Data.size()) &&
         "Invalid offset within MOS instruction for modifier!");
  char *RangeStart = Data.begin() + Offset;

  for (char &Out : make_range(RangeStart, RangeStart + Bytes)) {
    Out = Value & 0xff;
    Value = Value >> 8;
  }

  if (Bits % 8 == 0) {
    return;
  }
  unsigned RemainingBits = Bits - Bytes * 8;
  unsigned BitMask = (1 << RemainingBits) - 1;
  Data[Bytes + Offset] &= ~BitMask;
  Data[Bytes + Offset] |= (Value & BitMask);
}

bool PICMidMCAsmBackend::evaluateTargetFixup(const MCAssembler &Asm,
                                             const MCAsmLayout &Layout,
                                             const MCFixup &Fixup,
                                             const MCFragment *DF,
                                             const MCValue &Target,
                                             uint64_t &Value, bool &WasForced) {

  const bool IsPCAbs11 = Fixup.getKind() == (MCFixupKind)PICMid::PCAbs11;

  assert(IsPCAbs11 && "unexpected target fixup kind");

  Value = Target.getConstant();
  if (const MCSymbolRefExpr *A = Target.getSymA()) {
    const MCSymbol &Sym = A->getSymbol();
    if (Sym.isDefined()) {
      Value += Layout.getSymbolOffset(Sym);
    }
  }

  if (const MCSymbolRefExpr *B = Target.getSymB()) {
    const MCSymbol &Sym = B->getSymbol();
    if (Sym.isDefined()) {
      Value -= Layout.getSymbolOffset(Sym);
    }
  }

  //   Value -= Layout.getFragmentOffset(DF) + Fixup.getOffset();

  assert(Value % 2 == 0 &&
         "Program memory address in bytes must be evenly divisible by 2");
  Value /= 2;

  return false;
}

bool PICMidMCAsmBackend::fixupNeedsRelaxation(const MCFixup &Fixup,
                                              uint64_t Value,
                                              const MCRelaxableFragment *DF,
                                              const MCAsmLayout &Layout) const {
  return false;
}

bool PICMidMCAsmBackend::fixupNeedsRelaxationAdvanced(
    const MCFixup &Fixup, bool Resolved, uint64_t Value,
    const MCRelaxableFragment *DF, const MCAsmLayout &Layout,
    const bool WasForced) const {
  //   return Fixup.getKind() == (MCFixupKind)PICMid::PCAbs11;
  return false;
}

bool PICMidMCAsmBackend::writeNopData(raw_ostream &OS, uint64_t Count,
                                      const MCSubtargetInfo *STI) const {
  for (uint64_t i = 0; i < Count; ++i) {
    OS << '\0' << '\0';
  }
  return true;
}

MCAsmBackend *createPICMidAsmBackend(const Target &T,
                                     const MCSubtargetInfo &STI,
                                     const MCRegisterInfo &MRI,
                                     const MCTargetOptions &TO) {
  return new PICMidMCAsmBackend(STI.getTargetTriple().getOS());
}

} // namespace llvm
