#include "PICMidMCTargetDesc.h"
#include "PICMidMCAsmBackend.h"
#include "PICMidMCAsmInfo.h"
#include "PICMidMCCodeEmitter.h"
#include "PICMidMCInstPrinter.h"
#include "PICMidMCObjectFileInfo.h"
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

#define GET_SUBTARGETINFO_MC_DESC
#include "PICMidGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "PICMidGenRegisterInfo.inc"

namespace llvm {

// Internally used functions
static MCRegisterInfo *createPICMidMCRegisterInfo(const Triple &tt) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitPICMidMCRegisterInfo(X, 0);
  return X;
}

static MCSubtargetInfo *
createPICMidMCSubtargetInfo(const Triple &tt, StringRef cpu, StringRef fs) {
  return createPICMidMCSubtargetInfoImpl(tt, cpu, cpu, fs);
}

MCInstrInfo *createPICMidMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitPICMidMCInstrInfo(X);

  return X;
}

static MCInstPrinter *createPICMCInstPrinter(const Triple &T,
                                             unsigned SyntaxVariant,
                                             const MCAsmInfo &MAI,
                                             const MCInstrInfo &MII,
                                             const MCRegisterInfo &MRI) {
  switch (SyntaxVariant) {
  case 0:
    return new PICMidMCInstPrinter(MAI, MII, MRI);
  default:
    return nullptr;
  }
}

static MCObjectFileInfo *
createPICMidMCObjectFileInfo(MCContext &Ctx, bool PIC,
                             bool LargeCodeModel = false) {
  MCObjectFileInfo *MOFI = new PICMidMCObjectFileInfo();
  MOFI->initMCObjectFileInfo(Ctx, PIC, LargeCodeModel);
  return MOFI;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePICMidTargetMC() {
  RegisterMCAsmInfo<PICMidMCAsmInfo> X(getThePICMidTarget());

  TargetRegistry::RegisterMCInstrInfo(getThePICMidTarget(),
                                      createPICMidMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(getThePICMidTarget(),
                                    createPICMidMCRegisterInfo);
  TargetRegistry::RegisterMCInstPrinter(getThePICMidTarget(),
                                        createPICMCInstPrinter);
  TargetRegistry::RegisterMCSubtargetInfo(getThePICMidTarget(),
                                          createPICMidMCSubtargetInfo);
  TargetRegistry::RegisterMCCodeEmitter(getThePICMidTarget(),
                                        createPICMidMCCodeEmitter);
  TargetRegistry::RegisterMCAsmBackend(getThePICMidTarget(),
                                       createPICMidAsmBackend);
  TargetRegistry::RegisterMCObjectFileInfo(getThePICMidTarget(),
                                           createPICMidMCObjectFileInfo);
}

} // namespace llvm
