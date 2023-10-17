#include "llvm/Support/Compiler.h"
#include "llvm/MC/MCParser/MCTargetAsmParser.h"


namespace llvm {

class PICMidAsmParser : public MCTargetAsmParser {
#define GET_ASSEMBLER_HEADER
#include "PICMidGenAsmMatcher.inc"
};

extern "C" LLVM_EXTERNAL_VISIBILITY void
LLVMInitializePICMidAsmParser() { // NOLINT
}

} // namespace llvm
