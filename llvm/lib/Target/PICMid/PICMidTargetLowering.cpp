#include "PICMidTargetLowering.h"
#include "PICMidTargetMachine.h"

using namespace llvm;

llvm::PICMidTargetLowering::PICMidTargetLowering(const PICMidTargetMachine &TM,
                                                 const PICMidSubtarget &STI)
    : TargetLowering(TM) {}
