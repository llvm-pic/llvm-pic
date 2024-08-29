#include "PICBaseMCTargetDesc.h"

#include "TargetInfo/PICBaseTargetInfo.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "PICBaseGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "PICBaseGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "PICBaseGenRegisterInfo.inc"

using namespace llvm;

MCInstrInfo *llvm::createPICBaseMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitPICBaseMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createPICBaseMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitPICBaseMCRegisterInfo(X, 0);

  return X;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePICBaseTargetMC() {
  TargetRegistry::RegisterMCInstrInfo(getThePICBaseTarget(),
                                      createPICBaseMCInstrInfo);

  TargetRegistry::RegisterMCRegInfo(getThePICBaseTarget(),
                                    createPICBaseMCRegisterInfo);
}
