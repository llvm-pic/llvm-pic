#include "PICMidInstructionSelector.h"

using namespace llvm;

namespace {

#define GET_GLOBALISEL_PREDICATE_BITSET
#include "PICMidGenGlobalISel.inc"

class PICMidInstructionSelector : public InstructionSelector {
public:
  PICMidInstructionSelector(const PICMidTargetMachine &TM, PICMidSubtarget &STI,
                            PICMidRegisterBankInfo &RBI);
  bool select(MachineInstr &I) override;

private:
  const PICMidSubtarget &STI;
  const PICMidRegisterBankInfo &RBI;
};

PICMidInstructionSelector::PICMidInstructionSelector(
    const PICMidTargetMachine &TM, PICMidSubtarget &STI,
    PICMidRegisterBankInfo &RBI)
    : STI(STI), RBI(RBI) {}

bool PICMidInstructionSelector::select(MachineInstr &I) { return false; }

} // anonymous namespace

InstructionSelector *
llvm::createPICMidInstructionSelector(const PICMidTargetMachine &TM,
                                      PICMidSubtarget &STI,
                                      PICMidRegisterBankInfo &RBI) {
  return new PICMidInstructionSelector(TM, STI, RBI);
}
