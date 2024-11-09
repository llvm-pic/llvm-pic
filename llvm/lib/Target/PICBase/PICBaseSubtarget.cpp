#include "PICBaseSubtarget.h"
#include "MCTargetDesc/PICBaseMCTargetDesc.h"

using namespace llvm;

#define DEBUG_TYPE "picbase-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "PICBaseGenSubtargetInfo.inc"

llvm::PICBaseSubtarget::PICBaseSubtarget(const Triple &TT,
                                         const std::string &CPU,
                                         const std::string &FS,
                                         const PICBaseTargetMachine &TM)
    : PICBaseGenSubtargetInfo(TT, CPU, CPU, FS) {}
