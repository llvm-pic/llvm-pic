#include "PICMidLegalizerInfo.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/CodeGen/TargetOpcodes.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Type.h"
#include "../PICMidSubtarget.h"

using namespace llvm;

PICMidLegalizerInfo::PICMidLegalizerInfo(const PICMidSubtarget &ST) {

    using namespace TargetOpcode;

    getActionDefinitions(G_ADD);

};
