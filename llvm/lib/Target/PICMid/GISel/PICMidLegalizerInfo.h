#ifndef LLVM_LIB_TARGET_PICMid_PICMidMACHINELEGALIZER_H
#define LLVM_LIB_TARGET_PICMid_PICMidMACHINELEGALIZER_H

#include "llvm/CodeGen/GlobalISel/LegalizerInfo.h"
#include <llvm/CodeGen/GlobalISel/Legalizer.h>

namespace llvm {

    class PICMidSubtarget;

    class PICMidLegalizerInfo : public LegalizerInfo {
        public:
        PICMidLegalizerInfo(const PICMidSubtarget &ST);
    };

} // namespace llvm
#endif