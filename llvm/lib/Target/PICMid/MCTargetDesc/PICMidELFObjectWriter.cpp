#include "PICMidELFObjectWriter.h"
#include "PICMidFixupKinds.h"

#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCValue.h"

namespace llvm {

PICMidELFObjectWriter::PICMidELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(false, OSABI, ELF::EM_MCHP_PIC, true) {}

unsigned PICMidELFObjectWriter::getRelocType(MCContext &Ctx,
                                             const MCValue &Target,
                                             const MCFixup &Fixup,
                                             bool IsPCRel) const {
//   MCSymbolRefExpr::VariantKind Modifier = Target.getAccessVariant();
  switch ((unsigned)Fixup.getKind()) {
  case PICMid::Imm1:
    return ELF::R_PICMID_IMM1;
  case PICMid::Imm3:
    return ELF::R_PICMID_IMM3;
  case PICMid::Imm8:
    return ELF::R_PICMID_IMM8;
  case PICMid::Addr7:
    return ELF::R_PICMID_ADDR7;
  case PICMid::PCAbs11:
    return ELF::R_PICMID_PCABS11;
  default:
    llvm_unreachable("invalid fixup kind!");
  }
  return 0;
}

} // namespace llvm
