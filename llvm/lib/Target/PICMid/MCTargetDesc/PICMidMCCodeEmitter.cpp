#include "PICMidMCCodeEmitter.h"

#include "MCTargetDesc/PICMidMCExpr.h"
#include "MCTargetDesc/PICMidMCTargetDesc.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {

#define GET_INSTRMAP_INFO
#include "PICMidGenInstrInfo.inc"
#undef GET_INSTRMAP_INFO

PICMidMCCodeEmitter::PICMidMCCodeEmitter(const MCInstrInfo &MCII,
                                         MCContext &Ctx)
    : mcii(MCII), ctx(Ctx) {}

static void emitLittleEndian(uint64_t BinaryOpCode, unsigned Size,
                             raw_ostream &OS) {
  for (unsigned i = 0; i < Size; ++i) {
    char next = (BinaryOpCode & 0xFF);
    BinaryOpCode >>= 8;
    OS << next;
  }
}

void PICMidMCCodeEmitter::encodeInstruction(const MCInst &Inst, raw_ostream &OS,
                                            SmallVectorImpl<MCFixup> &Fixups,
                                            const MCSubtargetInfo &STI) const {
  const MCInstrDesc &Desc = mcii.get(Inst.getOpcode());
  // Get byte count of instruction
  unsigned Size = Desc.getSize();

  assert(Size > 0 && "Instruction size cannot be zero");
  uint64_t BinaryOpCode = getBinaryCodeForInstr(Inst, Fixups, STI);
  emitLittleEndian(BinaryOpCode, Size, OS);
}

template <PICMid::Fixups Fixup, unsigned Offset>
unsigned PICMidMCCodeEmitter::encodeImm(const MCInst &MI, unsigned OpNo,
                                        SmallVectorImpl<MCFixup> &Fixups,
                                        const MCSubtargetInfo &STI) const {
  auto MO = MI.getOperand(OpNo);

  if (!MO.isExpr()) {
    assert(MO.isImm());
    return MO.getImm();
  }

  if (isa<PICMidMCExpr>(MO.getExpr())) {
    assert(0 && "To be implemented!!!");
    return getExprOpValue(MO.getExpr(), Fixups, STI, Offset);
  }

  MCFixupKind FixupKind =
      static_cast<MCFixupKind>(Fixup); // TODO: How does this work ?
  //   MCFixupKind FixupKind = static_cast<MCFixupKind>(
  //       PICMidFixupKinds::getFixupKindInfo(Fixup, nullptr));
  Fixups.push_back(
      MCFixup::create(Offset, MO.getExpr(), FixupKind, MI.getLoc()));

  return 0;
}

unsigned
PICMidMCCodeEmitter::getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                                       SmallVectorImpl<MCFixup> &Fixups,
                                       const MCSubtargetInfo &STI) const {
  if (MO.isImm())
    return MO.getImm();

  assert(MO.isExpr());

  const MCExpr *Expr = MO.getExpr();
  if (isa<MCSymbolRefExpr>(Expr)) {
    Fixups.push_back(MCFixup::create(0, Expr, MCFixupKind::FK_Data_1));
    return 0;
  }

  int64_t Res;
  if (Expr->evaluateAsAbsolute(Res)) {
    return Res;
  }

  llvm_unreachable("Unhandled expression!");
  return 0;
}

#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "PICMidGenMCCodeEmitter.inc"

MCCodeEmitter *createPICMidMCCodeEmitter(const MCInstrInfo &mcii,
                                         MCContext &ctx) {
  return new PICMidMCCodeEmitter(mcii, ctx);
}

} // namespace llvm
