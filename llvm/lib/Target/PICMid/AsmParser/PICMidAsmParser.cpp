#include "MCTargetDesc/PICMidMCExpr.h"
#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "PICMidOperand.h"
#include "PICMidSubtarget.h"
#include "TargetInfo/PICMidTargetInfo.h"

#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCParser/MCAsmLexer.h"
#include "llvm/MC/MCParser/MCTargetAsmParser.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"
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

  ParseStatus parseDirective(AsmToken DirectiveID) override {
    // TODO: Implement own asm directives
    return ParseStatus{ParseStatus::NoMatch};
  }

  bool parseRegister(MCRegister &Reg, SMLoc &StartLoc, SMLoc &EndLoc) override {
    auto result = tryParseRegister(Reg, StartLoc, EndLoc);

    return result != MatchOperand_Success;
  }

  OperandMatchResultTy tryParseRegister(MCRegister &Reg, SMLoc &StartLoc,
                                        SMLoc &EndLoc) override {
    std::string AnyCase(StartLoc.getPointer(),
                        EndLoc.getPointer() - StartLoc.getPointer());
    std::transform(AnyCase.begin(), AnyCase.end(), AnyCase.begin(),
                   [](unsigned char C) { return std::tolower(C); });
    StringRef RegisterName(AnyCase.c_str(), AnyCase.size());
    Reg = MCRegister(MatchRegisterName(RegisterName));

    return Reg.isValid() ? MatchOperand_Success : MatchOperand_NoMatch;
  }

  bool ParseInstruction(ParseInstructionInfo &Info, StringRef Name,
                        SMLoc NameLoc, OperandVector &Operands) override {
    Operands.push_back(PICMidOperand::createToken(sti, Name, NameLoc));

    if (getLexer().is(AsmToken::EndOfStatement)) {
      return false;
    }

    if (ParseOperand(Operands)) {
      return true;
    }

    while (getLexer().isNot(AsmToken::EndOfStatement)) {
      if (getLexer().isNot(AsmToken::Comma)) {
        return Error(getTok().getLoc(), "missing delimiter, i.e., comma");
      }

      // Consume comma
      getLexer().Lex();
      if (ParseOperand(Operands)) {
        return true;
      }
    }

    return false;
  }

  bool ParseRegister(OperandVector &Operands) {
    MCRegister reg = 0;
    SMLoc s = getLexer().getLoc();
    SMLoc e = getLexer().getTok().getEndLoc();

    if (tryParseRegister(reg, s, e) != MatchOperand_Success) {
      return true;
    }

    Operands.push_back(PICMidOperand::createReg(sti, reg, s, e));
    return false;
  }

  bool ParseImmediate(OperandVector &Operands) {
    SMLoc startLoc = getLexer().getLoc();
    SMLoc endLoc;
    const MCExpr *expr;
    if (parsePrimaryExpr(expr, endLoc)) {
      return true;
    }

    // Is program memory address
    if (const auto *SymbolRef = dyn_cast<MCSymbolRefExpr>(expr)) {
      // asm("nop");
      // Operands.push_back(PICMidOperand::createImm(sti, expr->))
      expr = PICMidMCExpr::create(PICMidMCExpr::VK_PICMID_PCABS11, SymbolRef,
                                  false, getContext());
    }

    // Is literal
    Operands.push_back(PICMidOperand::createImm(sti, expr, startLoc, endLoc));
    return false;
  }

  bool ParseOperand(OperandVector &Operands) {
    return (ParseImmediate(Operands) && ParseRegister(Operands));
  }
};

#define GET_MATCHER_IMPLEMENTATION
#include "PICMidGenAsmMatcher.inc"

extern "C" LLVM_EXTERNAL_VISIBILITY void
LLVMInitializePICMidAsmParser() { // NOLINT
  RegisterMCAsmParser<PICMidAsmParser> X(getThePICMidTarget());
}

} // namespace llvm
