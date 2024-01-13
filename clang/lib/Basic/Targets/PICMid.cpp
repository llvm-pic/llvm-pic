#include "PICMid.h"
#include "Targets.h"
#include "clang/Basic/Builtins.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringSwitch.h"

using namespace clang;
using namespace clang::targets;
using namespace llvm;

const char *const PICMidTargetInfo::GCCRegNames[] = {
    "r0", "r1", "r2",  "r3",  "r4",  "r5",  "r6",  "r7",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};

ArrayRef<const char *> PICMidTargetInfo::getGCCRegNames() const {
  return llvm::ArrayRef(GCCRegNames);
}

void PICMidTargetInfo::getTargetDefines(const LangOptions &Opts,
                                        MacroBuilder &Builder) const {
  // TODO: target macros
}
