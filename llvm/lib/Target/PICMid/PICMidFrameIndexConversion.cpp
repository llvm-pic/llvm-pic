#include "PICMidFrameIndexConversion.h"

#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "PICMid.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"

#define DEBUG_TYPE "picmid-frame-index-conversion"

using namespace llvm;

namespace {

class PICMidFrameIndexConversion : public MachineFunctionPass {
public:
  static char ID;

  PICMidFrameIndexConversion() : MachineFunctionPass(ID) {
    initializePICMidFrameIndexConversionPass(*PassRegistry::getPassRegistry());
  }

  bool runOnMachineFunction(MachineFunction &MF) override;
};

bool PICMidFrameIndexConversion::runOnMachineFunction(MachineFunction &MF) {
  bool Changed = false;

  for (MachineBasicBlock &MBB : MF) {
    SmallVector<MachineBasicBlock::iterator> FrameIndicesToRemove = {};
    for (auto It = MBB.begin(); It != MBB.end(); ++It) {
      if (It->getOpcode() != PICMid::G_FRAME_INDEX) {
        continue;
      }

      MachineIRBuilder Builder(*It);

      Changed = true;
      Builder.buildInstr(PICMid::G_CONSTANT, {}, {})
          .addDef(It->getOperand(0).getReg())
          .add(It->getOperand(1));

      FrameIndicesToRemove.push_back(It);
    }
    for (const auto &FrameIndex : FrameIndicesToRemove) {
      FrameIndex->removeFromParent();
    }
  }

  return Changed;
}

} // namespace

char PICMidFrameIndexConversion::ID = 0;

INITIALIZE_PASS(PICMidFrameIndexConversion, DEBUG_TYPE,
                "Convert frame indices into constant values", false, false)

MachineFunctionPass *llvm::createPICMidFrameIndexConversion() {
  return new PICMidFrameIndexConversion();
}
