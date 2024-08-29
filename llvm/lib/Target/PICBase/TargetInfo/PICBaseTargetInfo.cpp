#include "PICBaseTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getThePICBaseTarget() {
  static Target ThePICBaseTarget;
  return ThePICBaseTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void
LLVMInitializePICBaseTargetInfo() { // NOLINT
  RegisterTarget<Triple::picbase> X(getThePICBaseTarget(), "picbase",
                                    "PIC(c) Baseline core devices", "PICBase");
}
