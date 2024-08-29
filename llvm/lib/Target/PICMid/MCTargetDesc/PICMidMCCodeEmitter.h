#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCCODEEMITTER_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCCODEEMITTER_H_

#include "PICMidFixupKinds.h"

#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
// #include "llvm/MC/MCOperand.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Support/DataTypes.h"

#define GET_INSTRINFO_OPERAND_TYPES_ENUM
#include "PICMidGenInstrInfo.inc"

namespace llvm {
class PICMidMCCodeEmitter : public MCCodeEmitter {
private:
  const MCInstrInfo &mcii;
  MCContext &ctx;

public:
  PICMidMCCodeEmitter(const MCInstrInfo &mcii, MCContext &ctx);

private:
  void encodeInstruction(const MCInst &Inst, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  unsigned getExprOpValue(const MCExpr *Expr, SmallVectorImpl<MCFixup> &Fixups,
                          const MCSubtargetInfo &STI,
                          unsigned int Offset) const;

  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  /// Encodes an immediate value with a given fixup.
  /// \tparam Offset The offset into the instruction for the fixup.
  template <PICMid::Fixups Fixup, unsigned Offset>
  unsigned encodeImm(const MCInst &MI, unsigned OpNo,
                     SmallVectorImpl<MCFixup> &Fixups,
                     const MCSubtargetInfo &STI) const;
};

MCCodeEmitter *createPICMidMCCodeEmitter(const MCInstrInfo &mcii,
                                         MCContext &ctx);

} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCCODEEMITTER_H_