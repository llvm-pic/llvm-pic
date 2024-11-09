#include "PICMidSubtarget.h"

#include "GISel/PICMidInstructionSelector.h"
#include "MCTargetDesc/PICMidMCTargetDesc.h"

#include "llvm/BinaryFormat/ELF.h"
#include "llvm/CodeGen/GlobalISel/CallLowering.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelector.h"
#include "llvm/CodeGen/GlobalISel/Utils.h"
#include "llvm/CodeGen/MachineScheduler.h"
#include "llvm/MC/TargetRegistry.h"

#define DEBUG_TYPE "picmid-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "PICMidGenSubtargetInfo.inc"

using namespace llvm;

PICMidSubtarget::PICMidSubtarget(const Triple &TT, const std::string &CPU,
                                 const std::string &FS,
                                 const PICMidTargetMachine &TM)
    : PICMidGenSubtargetInfo(TT, CPU, CPU, FS), RegBankInfo(),
      CallLoweringInfo(&TargetLoweringInfo), RegisterInfo(), Legalizer(),
      TargetLoweringInfo(TM, *this),
      InstSelector(createPICMidInstructionSelector(TM, *this, RegBankInfo)) {}
