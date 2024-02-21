#include "PICMidInstrInfo.h"

#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "PICMidFrameLowering.h"

#include "PICMidSubtarget.h"

using namespace llvm;

#define DEBUG_TYPE "picmid-instrinfo"

#define GET_INSTRINFO_CTOR_DTOR
#include "PICMidGenInstrInfo.inc"
#undef GET_INSTRINFO_CTOR_DTOR

llvm::PICMidInstrInfo::PICMidInstrInfo()
    : PICMidGenInstrInfo() //
{}
