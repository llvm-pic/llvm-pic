#include "PICMidMCTargetDesc.h"
#include "TargetInfo/PICMidTargetInfo.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "PICMidGenInstrInfo.inc"

// #define GET_SUBTARGETINFO_MC_DESC
// #include "PICMidGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "PICMidGenRegisterInfo.inc"

namespace llvm {

// Internally used functions
static MCRegisterInfo *createPICMidMCRegisterInfo(const Triple &tt) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitPICMidMCRegisterInfo(X, 0);
  return X;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePICMidTargetMC() {}

MCInstrInfo *createPICMidMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitPICMidMCInstrInfo(X);

  TargetRegistry::RegisterMCInstrInfo(getThePICMidTarget(), createPICMidMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(getThePICMidTarget(), createPICMidMCRegisterInfo);

  return X;
}

} // namespace llvm
