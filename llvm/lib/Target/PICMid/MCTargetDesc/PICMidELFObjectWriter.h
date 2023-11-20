#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDELFOBJECTWRITER_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDELFOBJECTWRITER_H_

#include "llvm/MC/MCELFObjectWriter.h"

namespace llvm {
class PICMidELFObjectWriter : public MCELFObjectTargetWriter {
public:
  explicit PICMidELFObjectWriter(uint8_t OSABI);
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
};


} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDELFOBJECTWRITER_H_