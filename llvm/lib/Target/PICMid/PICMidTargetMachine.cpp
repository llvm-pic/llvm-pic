#include "llvm/InitializePasses.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/PassRegistry.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Triple.h"
#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "TargetInfo/PICMidTargetInfo.h"
#include "PICMidTargetMachine.h"

using namespace llvm;
													
/// Processes a CPU name.
static StringRef getCPU(StringRef CPU) {
  return (CPU.empty() || CPU == "generic") ? "picmid" : CPU;
}

// TODO: Add Data Layout
static const char *PICMidDataLayout = "E-p:8:8-i:8:8";

PICMidTargetMachine::PICMidTargetMachine(
  const Target &T, const Triple &TT,
                                           StringRef CPU, StringRef FS,
                                           const TargetOptions &Options,
                                           std::optional<Reloc::Model> RM,
                                           std::optional<CodeModel::Model> CM,
                                           CodeGenOpt::Level OL,
                                           bool JIT)
      : LLVMTargetMachine(T, PICMidDataLayout, TT, getCPU(CPU), FS, Options,
													Reloc::Static,
                          CodeModel::Small, OL) {
                            setGlobalISel(true);
													}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePICMidTarget() {
	RegisterTargetMachine<PICMidTargetMachine> X(getThePICMidTarget());
	
	PassRegistry &PR = *PassRegistry::getPassRegistry();
	initializeGlobalISel(PR);

}
