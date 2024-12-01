#include "PICMidFrameIndexConversion.h"

#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "PICMid.h"
#include "PICMidSubtarget.h"

#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/CodeGen/GlobalISel/CSEInfo.h"
#include "llvm/CodeGen/GlobalISel/Combiner.h"
#include "llvm/CodeGen/GlobalISel/CombinerHelper.h"
#include "llvm/CodeGen/GlobalISel/GIMatchTableExecutor.h"
#include "llvm/CodeGen/GlobalISel/GIMatchTableExecutorImpl.h"
#include "llvm/CodeGen/GlobalISel/GISelKnownBits.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include "llvm/CodeGen/MachineDominators.h"
#include "llvm/CodeGen/TargetPassConfig.h"

#define GET_GICOMBINER_DEPS
#include "PICMidGenFrameIndexCombiner.inc"
#undef GET_GICOMBINER_DEPS

#define DEBUG_TYPE "picmid-frame-index-conversion"

using namespace llvm;

namespace {

#define GET_GICOMBINER_TYPES
#include "PICMidGenFrameIndexCombiner.inc"
#undef GET_GICOMBINER_TYPES

class PICMidFrameIndexCombinerImpl : public Combiner {
  mutable CombinerHelper Helper;
  const PICMidFrameIndexCombinerImplRuleConfig &RuleConfig;
  AAResults *AA;

public:
  PICMidFrameIndexCombinerImpl(
      MachineFunction &MF, CombinerInfo &CInfo, const TargetPassConfig *TPC,
      bool IsPreLegalize, GISelKnownBits &KB, GISelCSEInfo *CSEInfo,
      const PICMidFrameIndexCombinerImplRuleConfig &RuleConfig,
      const PICMidSubtarget &STI, MachineDominatorTree *MDT,
      const LegalizerInfo *LI, AAResults *AA);
  static const char *getName() { return "PICMidFrameIndexCombiner"; }
  bool tryCombineAll(MachineInstr &I) const override;

private:
#define GET_GICOMBINER_CLASS_MEMBERS
#include "PICMidGenFrameIndexCombiner.inc"
#undef GET_GICOMBINER_CLASS_MEMBERS
};

#define GET_GICOMBINER_IMPL
#include "PICMidGenFrameIndexCombiner.inc"
#undef GET_GICOMBINER_IMPL

PICMidFrameIndexCombinerImpl::PICMidFrameIndexCombinerImpl(
    MachineFunction &MF, CombinerInfo &CInfo, const TargetPassConfig *TPC,
    bool IsPreLegalize, GISelKnownBits &KB, GISelCSEInfo *CSEInfo,
    const PICMidFrameIndexCombinerImplRuleConfig &RuleConfig,
    const PICMidSubtarget &STI, MachineDominatorTree *MDT,
    const LegalizerInfo *LI, AAResults *AA)
    : Combiner(MF, CInfo, TPC, &KB, CSEInfo),
      Helper(Observer, B, IsPreLegalize, &KB, MDT, LI), RuleConfig(RuleConfig),
      AA(AA),
#define GET_GICOMBINER_CONSTRUCTOR_INITS
#include "PICMidGenFrameIndexCombiner.inc"
#undef GET_GICOMBINER_CONSTRUCTOR_INITS
{
} // namespace

class PICMidFrameIndexConversion : public MachineFunctionPass {
  PICMidFrameIndexCombinerImplRuleConfig RuleConfig;

public:
  static char ID;

  PICMidFrameIndexConversion() : MachineFunctionPass(ID) {
    initializePICMidFrameIndexConversionPass(*PassRegistry::getPassRegistry());
    if (!RuleConfig.parseCommandLineOption()) {
      report_fatal_error("Invalid rule identifier");
    }
  }

  StringRef getPassName() const override {
    return "PICMidFrameIndexConversion";
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

  void getAnalysisUsage(AnalysisUsage &AU) const override;
};

bool PICMidFrameIndexConversion::runOnMachineFunction(MachineFunction &MF) {
  if (MF.getProperties().hasProperty(
          MachineFunctionProperties::Property::FailedISel)) {
    return false; // Function has not changed
  }

  auto &TPC = getAnalysis<TargetPassConfig>();

  // Enable CSE.
  GISelCSEAnalysisWrapper &Wrapper =
      getAnalysis<GISelCSEAnalysisWrapperPass>().getCSEWrapper();
  auto *CSEInfo = &Wrapper.get(TPC.getCSEConfig());

  const PICMidSubtarget &ST = MF.getSubtarget<PICMidSubtarget>();
  const auto *LI = ST.getLegalizerInfo();

  const Function &F = MF.getFunction();
  bool EnableOpt =
      MF.getTarget().getOptLevel() != CodeGenOptLevel::None && !skipFunction(F);
  bool IsPreLegalize = !MF.getProperties().hasProperty(
      MachineFunctionProperties::Property::Legalized);
  GISelKnownBits *KB = &getAnalysis<GISelKnownBitsAnalysis>().get(MF);
  MachineDominatorTree *MDT =
      &getAnalysis<MachineDominatorTreeWrapperPass>().getDomTree();
  AAResults *AA = &getAnalysis<AAResultsWrapperPass>().getAAResults();
  CombinerInfo CInfo(
      /*AllowIllegalOps*/ IsPreLegalize, /*ShouldLegalizeIllegal*/ false,
      /*LegalizerInfo*/ nullptr, EnableOpt, F.hasOptSize(), F.hasMinSize());

  PICMidFrameIndexCombinerImpl Impl(MF, CInfo, &TPC, IsPreLegalize, *KB,
                                    CSEInfo, RuleConfig, ST, MDT, LI, AA);
  return Impl.combineMachineInstrs();
}

void PICMidFrameIndexConversion::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.addRequired<TargetPassConfig>();
  AU.setPreservesCFG();
  AU.addRequired<GISelKnownBitsAnalysis>();
  AU.addPreserved<GISelKnownBitsAnalysis>();
  AU.addRequired<MachineDominatorTreeWrapperPass>();
  AU.addPreserved<MachineDominatorTreeWrapperPass>();
  AU.addRequired<GISelCSEAnalysisWrapperPass>();
  AU.addPreserved<GISelCSEAnalysisWrapperPass>();
  AU.addRequired<AAResultsWrapperPass>();
  MachineFunctionPass::getAnalysisUsage(AU);
}

} // namespace

char PICMidFrameIndexConversion::ID = 0;

INITIALIZE_PASS_BEGIN(PICMidFrameIndexConversion, DEBUG_TYPE,
                      "Convert frame indices into constant values", false,
                      false)
INITIALIZE_PASS_DEPENDENCY(TargetPassConfig)
INITIALIZE_PASS_DEPENDENCY(GISelKnownBitsAnalysis)
INITIALIZE_PASS_END(PICMidFrameIndexConversion, DEBUG_TYPE,
                    "Convert frame indices into constant values", false, false)

MachineFunctionPass *llvm::createPICMidFrameIndexConversion() {
  return new PICMidFrameIndexConversion();
}
