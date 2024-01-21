#ifndef LLVM_LIB_TARGET_PICMid_PICMidMACHINELEGALIZER_H
#define LLVM_LIB_TARGET_PICMid_PICMidMACHINELEGALIZER_H

#include "llvm/CodeGen/GlobalISel/Legalizer.h"
#include "llvm/CodeGen/GlobalISel/LegalizerInfo.h"

namespace llvm {

class PICMidLegalizerInfo : public LegalizerInfo {
public:
  PICMidLegalizerInfo();
};

} // namespace llvm
#endif