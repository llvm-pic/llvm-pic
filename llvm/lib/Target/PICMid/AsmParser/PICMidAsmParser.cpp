#include "PICMidSubtarget.h"
#include "PICMidOperand.h"

#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCParser/MCTargetAsmParser.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/Compiler.h"

namespace llvm {

class PICMidAsmParser : public MCTargetAsmParser {
  const PICMidSubtarget &sti;
  MCAsmParser &parser;
  const MCRegisterInfo *mri;
  const std::string generateStubs = "gs";

#define GET_ASSEMBLER_HEADER
#include "PICMidGenAsmMatcher.inc"

public:
  enum MOSMatchResultType {
    Match_UnknownError = FIRST_TARGET_MATCH_RESULT_TY,
#define GET_OPERAND_DIAGNOSTIC_TYPES
#include "PICMidGenAsmMatcher.inc"
  };

  PICMidAsmParser(const MCSubtargetInfo &sti, MCAsmParser &parser,
                  const MCInstrInfo &mii, const MCTargetOptions &options)
      : MCTargetAsmParser(options, sti, mii),
        sti(static_cast<const PICMidSubtarget &>(sti)), parser(parser) {
    MCAsmParserExtension::Initialize(parser);
    mri = getContext().getRegisterInfo();

    // TODO: Add aliases
    // TODO: Add available features
    // TODO: Add ELF header flags
  }

  MCAsmLexer &getLexer() const { return parser.getLexer(); }
  MCAsmParser &getParser() const { return parser; }

  bool invalidOperand(const SMLoc &loc, const OperandVector &operands,
                      uint64_t errorInfo) {
    SMLoc errorLoc = loc;
    const char *diag = "invalid operand for instruction";

    if (errorInfo != ~0U) {
      if (errorInfo >= operands.size()) {
        diag = "too few operands for instruction";
      } else {
        errorLoc = operands[errorInfo]->getStartLoc();
      }
    }

    return Error(errorLoc, diag);
  }

  bool missingFeature(const SMLoc &loc, uint64_t errorInfo) {
    return Error(loc,
                 "instruction requires a CPU feature not currently enabled");
  }

  bool emit(MCInst &inst, const SMLoc &loc, MCStreamer &out) const {
    inst.setLoc(loc);
    out.emitInstruction(inst, sti);
    return false;
  }

  bool MatchAndEmitInstruction(SMLoc loc, unsigned &opcode,
                               OperandVector &operands, MCStreamer &out,
                               uint64_t &errorInfo,
                               bool matchingInlineAsm) override {
    MCInst inst;
    unsigned matchResult =
        MatchInstructionImpl(operands, inst, errorInfo, matchingInlineAsm);

    switch (matchResult) {
    case Match_Success:
      return emit(inst, loc, out);
    case Match_MissingFeature:
      return missingFeature(loc, errorInfo);
    case Match_InvalidOperand:
      return invalidOperand(loc, operands, errorInfo);
    case Match_MnemonicFail:
      return Error(loc, "invalid instruction");
    case Match_InvalidAddr7:
      return Error(loc, "operand must be an 7-bit address");
    case Match_InvalidPCAbs11:
      return Error(loc, "operand must be an 11-bit PC absolute address");
    case Match_immediate:
      return Error(
          loc,
          "operand must be a valid 1-bit, 3-bit, or 8-bit immediate value");
    case Match_NearMisses:
      return Error(loc, "found some near misses");

    default:
      return true;
    }
  }

};

extern "C" LLVM_EXTERNAL_VISIBILITY void
LLVMInitializePICMidAsmParser() { // NOLINT
}

} // namespace llvm
