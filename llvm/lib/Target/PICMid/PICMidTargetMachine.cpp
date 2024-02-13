#include "PICMidTargetMachine.h"
#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "TargetInfo/PICMidTargetInfo.h"
#include "llvm/CodeGen/GlobalISel/IRTranslator.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelect.h"
#include "llvm/CodeGen/GlobalISel/Legalizer.h"
#include "llvm/CodeGen/GlobalISel/RegBankSelect.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
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
                        Reloc::Static, CodeModel::Small, OL),
      SubTarget(TT, getCPU(CPU).str(), FS.str(), *this),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()) {
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

  bool addIRTranslator() override;
  bool addLegalizeMachineIR() override;
  bool addRegBankSelect() override;
  bool addGlobalInstructionSelect() override;
};

bool PICMidPassConfig::addIRTranslator() {
  addPass(new IRTranslator(getOptLevel()));
  return false;
}

bool PICMidPassConfig::addLegalizeMachineIR() {
  addPass(new Legalizer());
  return false;
}

bool PICMidPassConfig::addRegBankSelect() {
  addPass(new RegBankSelect());
  return false;
}

bool PICMidPassConfig::addGlobalInstructionSelect() {
  addPass(new InstructionSelect());
  return false;
}

} // namespace

TargetPassConfig *
llvm::PICMidTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new PICMidPassConfig(*this, PM);
}

const TargetSubtargetInfo *
llvm::PICMidTargetMachine::getSubtargetImpl(const Function &) const {
  return &SubTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePICMidTarget() {
  RegisterTargetMachine<PICMidTargetMachine> X(getThePICMidTarget());

  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializeGlobalISel(PR);
}
