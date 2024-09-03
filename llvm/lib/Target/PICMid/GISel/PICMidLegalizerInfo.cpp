#include "PICMidLegalizerInfo.h"
#include "GISel/PICMidRegisterBankInfo.h"
#include "MCTargetDesc/PICMidFixupKinds.h"
#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "PICMidRegisterInfo.h"
#include "PICMidSubtarget.h"
#include "llvm/CodeGen/GlobalISel/LegalizerHelper.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include "llvm/CodeGen/GlobalISel/Utils.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/RDFGraph.h"
#include "llvm/CodeGen/TargetOpcodes.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;
using namespace TargetOpcode;

PICMidLegalizerInfo::PICMidLegalizerInfo() {

  // Constants
  getActionDefinitionsBuilder(G_IMPLICIT_DEF).legalFor({S8}).unsupported();

  getActionDefinitionsBuilder(G_CONSTANT)
      .legalFor({S1, S8, P})
      .widenScalarToNextMultipleOf(0, 8)
      .maxScalar(0, S8)
      .unsupported();

  // required to be legal
  getActionDefinitionsBuilder({G_FRAME_INDEX, G_BLOCK_ADDR})
      .legalFor({P})
      .unsupported();

  // Integer Extension and Truncations

  getActionDefinitionsBuilder({
                                  G_SEXT,
                                  G_ZEXT,
                              })
      .widenScalarToNextMultipleOf(1, 8)
      .maxScalar(1, S8)
      .unsupported();

  getActionDefinitionsBuilder(G_TRUNC).maxScalar(0, S8).unsupported();

  // Type Conversion

  getActionDefinitionsBuilder(G_INTTOPTR).legalFor({P, S8}).unsupported();

  getActionDefinitionsBuilder(G_PTRTOINT).legalFor({S8, P}).unsupported();

  // Integer Operations

  getActionDefinitionsBuilder({G_ADD, G_SUB})
      .legalFor({S8, S1})
      .widenScalarToNextMultipleOf(0, 8)
      .maxScalar(0, S8)
      .unsupported();

  getActionDefinitionsBuilder({G_AND, G_OR, G_XOR})
      .legalFor({S8, S1})
      .widenScalarToNextMultipleOf(0, 8)
      .maxScalar(0, S8)
      .unsupported();

  // TODO implement libcalls or another form of custom call
  getActionDefinitionsBuilder({G_MUL, G_SDIV, G_UDIV, G_SREM, G_UREM})
      .unsupported();

  getActionDefinitionsBuilder(G_SELECT)
      .customFor({S8, S1, S8, S8})
      .unsupported();

  getActionDefinitionsBuilder({G_SHL, G_LSHR}).legalFor({S8}).unsupported();

  getActionDefinitionsBuilder(G_ICMP).customFor({S1, S8, S8}).unsupported();

  getActionDefinitionsBuilder(G_PTR_ADD)
      .legalForCartesianProduct({P, S8})
      .unsupported();

  // this does not need to be expanded in the legalizer, as it can be selected
  // via normal addition and increment
  getActionDefinitionsBuilder({G_UADDO, G_SADDO, G_USUBO, G_SSUBO})
      .legalFor({S8, S1, S8, S8})
      .unsupported();

  getActionDefinitionsBuilder({G_UADDE, G_SADDE, G_USUBE, G_SSUBE})
      .legalFor({S8, S1, S8, S8, S1})
      .unsupported();

  // We can absolutely select this, by checking the first bit, and then
  // optionally perfoming a bit flip.
  getActionDefinitionsBuilder(G_ABS).legalFor({S8}).unsupported();

  // unsupport all float, as well as G_FCONSTANT
  getActionDefinitionsBuilder(
      {G_FCONSTANT,    G_FCMP,     G_FNEG,      G_FPEXT,      G_FPTRUNC,
       G_FPTOSI,       G_FPTOUI,   G_FPTOSI,    G_FPTOUI,     G_SITOFP,
       G_UITOFP,       G_FABS,     G_FCOPYSIGN, G_FMINIMUM,   G_FMAXIMUM,
       G_FMINNUM_IEEE, G_FMINIMUM, G_FMAXIMUM,  G_FADD,       G_FSUB,
       G_FSUB,         G_FMUL,     G_FDIV,      G_FREM,       G_FMA,
       G_FMAD,         G_FPOW,     G_FEXP,      G_FEXP2,      G_FLOG,
       G_FLOG2,        G_FLOG10,   G_FCEIL,     G_FCOS,       G_FSIN,
       G_FSQRT,        G_FFLOOR,   G_FRINT,     G_FNEARBYINT, G_INTRINSIC_TRUNC,
       G_LROUND,       G_LLROUND})
      .unsupported();

  // Vector specific operations

  // Vector reductions operations

  // Memory operations

  getActionDefinitionsBuilder({G_LOAD, G_STORE})
      .legalFor({S8, S8, S8})
      .unsupported();

  // Fence will never appear as PIC is single thread
  getActionDefinitionsBuilder(G_FENCE).unsupported();

  // Control Flow

  getActionDefinitionsBuilder(G_PHI).alwaysLegal();

  getActionDefinitionsBuilder(G_BR).alwaysLegal();

  getActionDefinitionsBuilder(G_BRCOND).customFor({S1}).unsupported();

  // Variadic Arguments

  // Other Operations

  // Optimization Hints

  // Miscellaneous
  getLegacyLegalizerInfo().computeTables();
};

bool PICMidLegalizerInfo::legalizeCustom(
    LegalizerHelper &Helper, MachineInstr &MI,
    LostDebugLocObserver &LocObserver) const {
  MachineRegisterInfo &MRI = MI.getMF()->getRegInfo();
  switch (MI.getOpcode()) {
  case G_ICMP:
    return legalizeICmp(Helper, MI, MRI);
  case G_SELECT:
    return legalizeSelect(Helper, MI);
  case G_BRCOND:
    return legalizeBrCond(Helper, MI, MRI);
  default:
    return false;
  }
}

bool PICMidLegalizerInfo::legalizeBrCond(LegalizerHelper &Helper,
                                         MachineInstr &MI,
                                         MachineRegisterInfo &MRI) const {
  auto Cond = MI.getOperand(0);
  Register Dst = Cond.getReg();
  assert(MRI.getType(Dst) == S1);
  MachineBasicBlock *Dest = MI.getOperand(1).getMBB();
  MachineIRBuilder MIRBuilder = Helper.MIRBuilder;
  MachineInstr *DefMI = MRI.getVRegDef(Dst);
  auto CondOpc = DefMI->getOpcode();
  Register Arg1, Arg2;
  LLT DstTy, Arg1Ty, Arg2Ty;

  // icmp
  //  The Condition, if high (1) is true and should execute the next instruction
  CmpInst::Predicate Pred;
  Register Result2;

  switch (CondOpc) {
  case G_ICMP:
    Arg1 = DefMI->getOperand(2).getReg();
    Arg2 = DefMI->getOperand(3).getReg();
    Result2 = MRI.createGenericVirtualRegister(S1);

    Pred = static_cast<CmpInst::Predicate>(DefMI->getOperand(1).getPredicate());
    // at this point it should only be predicates for integer comparison
    switch (Pred) {
    case CmpInst::ICMP_EQ:
      MIRBuilder.buildInstr(PICMid::G_CBISZ).addUse(Dst).addMBB(Dest);
      break;
    case CmpInst::ICMP_NE:
      MIRBuilder.buildInstr(PICMid::G_CBICZ).addUse(Dst).addMBB(Dest);
      break;
    case CmpInst::ICMP_SLE:
    case CmpInst::ICMP_ULE:
      MIRBuilder.buildInstr(PICMid::G_CBISC).addUse(Dst).addMBB(Dest);
      break;
    case CmpInst::ICMP_SLT:
    case CmpInst::ICMP_ULT:
      MIRBuilder.buildInstr(PICMid::G_CBISC).addUse(Dst).addMBB(Dest);

      MIRBuilder.buildInstr(PICMid::G_CBICC).addUse(Result2).addMBB(Dest);

      MIRBuilder.setInsertPt(MIRBuilder.getMBB(), DefMI);
      MIRBuilder.buildICmp(Pred, Result2, Arg1, Arg2);
      break;

    // For these four cases, we can assume that they will be legalized. Therefor
    // we can assume the post legalization state of the comparison register, and
    // use the corresponding registers. This does not affect legalization of
    // G_ICMP.
    case CmpInst::ICMP_UGT:
    case CmpInst::ICMP_SGT:
      MIRBuilder.buildInstr(PICMid::G_CBICC).addUse(Dst).addMBB(Dest);
      break;
    case CmpInst::ICMP_UGE:
    case CmpInst::ICMP_SGE:
      MIRBuilder.buildInstr(PICMid::G_CBICC).addUse(Dst).addMBB(Dest);

      MIRBuilder.buildInstr(PICMid::G_CBISZ).addUse(Dst).addMBB(Dest);

      MIRBuilder.setInsertPt(MIRBuilder.getMBB(), DefMI);
      MIRBuilder.buildICmp(Pred, Result2, Arg1, Arg2);
      break;
    default:
      return false;
    }
    break;
  case G_EXTRACT:
  case G_TRUNC:
    // 1 is true
    std::tie(Dst, DstTy, Arg1, Arg1Ty) = MI.getFirst2RegLLTs();

    assert(DstTy == S1 && "Dst can only be an S1.");

    return false;
  default:
    return false;
  }
  MI.eraseFromParent();
  return true;
}

bool PICMidLegalizerInfo::legalizeSelect(LegalizerHelper &Helper,
                                         MachineInstr &MI) const {
  auto [Dst, Cond, Arg1, Arg2] = MI.getFirst4Regs();
  return false;
}

bool PICMidLegalizerInfo::legalizeICmp(LegalizerHelper &Helper,
                                       MachineInstr &MI,
                                       MachineRegisterInfo &MRI) const {
  auto Dst = MI.getOperand(0).getReg();
  auto Arg1 = MI.getOperand(2).getReg();
  auto Arg2 = MI.getOperand(3).getReg();
  MachineIRBuilder MIRBuilder = Helper.MIRBuilder;
  CmpInst::Predicate Pred =
      static_cast<CmpInst::Predicate>(MI.getOperand(1).getPredicate());
  switch (Pred) {
  case CmpInst::FCMP_FALSE:
  case CmpInst::FCMP_OEQ:
  case CmpInst::FCMP_OGT:
  case CmpInst::FCMP_OGE:
  case CmpInst::FCMP_OLT:
  case CmpInst::FCMP_OLE:
  case CmpInst::FCMP_ONE:
  case CmpInst::FCMP_ORD:
  case CmpInst::FCMP_UNO:
  case CmpInst::FCMP_UEQ:
  case CmpInst::FCMP_UGT:
  case CmpInst::FCMP_UGE:
  case CmpInst::FCMP_ULT:
  case CmpInst::FCMP_ULE:
  case CmpInst::FCMP_UNE:
  case CmpInst::FCMP_TRUE:
  case CmpInst::BAD_FCMP_PREDICATE:
    return false;
  case CmpInst::ICMP_EQ:
  case CmpInst::ICMP_NE:
  case CmpInst::ICMP_ULT:
  case CmpInst::ICMP_ULE:
  case CmpInst::ICMP_SLT:
  case CmpInst::ICMP_SLE:
    // TODO built the instruction
    // this is simply legal by later selection subtraction
    break;

  case CmpInst::ICMP_UGT:
    MIRBuilder.buildICmp(CmpInst::ICMP_ULE, Dst, Arg2, Arg1);
    MI.eraseFromParent();
    break;
  case CmpInst::ICMP_UGE:
    MIRBuilder.buildICmp(CmpInst::ICMP_ULT, Dst, Arg2, Arg1);
    MI.eraseFromParent();
    break;
  case CmpInst::ICMP_SGT:
    MIRBuilder.buildICmp(CmpInst::ICMP_SLE, Dst, Arg2, Arg1);
    MI.eraseFromParent();
    break;
  case CmpInst::ICMP_SGE:
    MIRBuilder.buildICmp(CmpInst::ICMP_SLT, Dst, Arg2, Arg1);
    MI.eraseFromParent();
    break;
    // swap operands
  case CmpInst::BAD_ICMP_PREDICATE:
    return false;
  }
  return true;
}