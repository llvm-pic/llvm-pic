include "llvm/Target/Target.td"
include "llvm/Target/GlobalISel/Combine.td"

include "PICMidInstrInfo.td"
include "PICMidRegisterInfo.td"

def PICMidInstrInfo : InstrInfo {
    let guessInstructionProperties = 0;
}

def PICMid : Target {
    let InstructionSet = PICMidInstrInfo;
}