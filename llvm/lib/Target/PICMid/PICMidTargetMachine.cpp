#include "PICMidTargetMachine.h"
#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "TargetInfo/PICMidTargetInfo.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/InitializePasses.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Pass.h"
#include "llvm/PassRegistry.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

/// Processes a CPU name.
static StringRef getCPU(StringRef CPU) {
  return (CPU.empty() || CPU == "generic") ? "pic16f88x" : CPU;
}

// TODO: Add Data Layout
static const char *PICMidDataLayout = "E-p:8:8-i:8:8";

PICMidTargetMachine::PICMidTargetMachine(const Target &T, const Triple &TT,
                                         StringRef CPU, StringRef FS,
                                         const TargetOptions &Options,
                                         std::optional<Reloc::Model> RM,
                                         std::optional<CodeModel::Model> CM,
                                         CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, PICMidDataLayout, TT, getCPU(CPU), FS, Options,
                        Reloc::Static, CodeModel::Small, OL) {
  initAsmInfo();
  setGlobalISel(true);
}

namespace {

class PICMidPassConfig : public TargetPassConfig {
public:
  PICMidPassConfig(PICMidTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  PICMidTargetMachine &getPICMidTargetMachine() const {
    return getTM<PICMidTargetMachine>();
  }
};

} // namespace

TargetPassConfig *
llvm::PICMidTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new PICMidPassConfig(*this, PM);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePICMidTarget() {
  RegisterTargetMachine<PICMidTargetMachine> X(getThePICMidTarget());

  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializeGlobalISel(PR);
}
