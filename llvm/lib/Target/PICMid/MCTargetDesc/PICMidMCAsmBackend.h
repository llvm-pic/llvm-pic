#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCASMBACKEND_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCASMBACKEND_H_

#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/TargetParser/Triple.h"

namespace llvm {

class PICMidMCAsmBackend : public MCAsmBackend {
private:
  Triple::OSType OSType;

public:
  PICMidMCAsmBackend(Triple::OSType OSType);

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override;

  unsigned getNumFixupKinds() const override;

  /// Get information on a fixup kind.
  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override;

  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override;

  /// Apply target-specific fixup, i.e., annotated with
  /// MCFixupKindInfo::FKF_IsTarget
  ///
  /// FIXME:    In its current form, this function may be redundant.
  ///           That's because it always returns false which is why
  ///           Value is ignored.
  /// TODO:     Decide, what to do with this function
  ///
  /// @return bool If true, fixup may be resolved prior to linking stage.
  ///               In that case, the fixup is not listed as a relocation
  ///               entry.
  bool evaluateTargetFixup(const MCAssembler &Asm, const MCAsmLayout &Layout,
                           const MCFixup &Fixup, const MCFragment *DF,
                           const MCValue &Target, uint64_t &Value,
                           bool &WasForced) override;

  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                            const MCRelaxableFragment *DF,
                            const MCAsmLayout &Layout) const override;

  bool fixupNeedsRelaxationAdvanced(const MCFixup &Fixup, bool Resolved,
                                    uint64_t Value,
                                    const MCRelaxableFragment *DF,
                                    const MCAsmLayout &Layout,
                                    const bool WasForced) const override;

  bool writeNopData(raw_ostream &OS, uint64_t Count,
                    const MCSubtargetInfo *STI) const override;
};

MCAsmBackend *createPICMidAsmBackend(const Target &T,
                                     const MCSubtargetInfo &STI,
                                     const MCRegisterInfo &MRI,
                                     const MCTargetOptions &TO);

} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCASMBACKEND_H_