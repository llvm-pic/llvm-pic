#include "PICMidLegalizerInfo.h"
#include "GISel/PICMidRegisterBankInfo.h"
#include "MCTargetDesc/PICMidFixupKinds.h"
#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "PICMidRegisterInfo.h"
#include "PICMidSubtarget.h"
#include "llvm/CodeGen/GlobalISel/LegalizerHelper.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RDFGraph.h"
#include "llvm/CodeGen/TargetOpcodes.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Type.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <cassert>

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

  // INFO this should only effect memory operations is therefor correct
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

  getActionDefinitionsBuilder(G_ICMP).legalFor({S1, S8, S8}).unsupported();

  getActionDefinitionsBuilder(G_PTR_ADD)
      .legalForCartesianProduct({P, S8})
      .unsupported();

  // TODO support by implementing a virtual "carryadd" instruction
  getActionDefinitionsBuilder({G_UADDE, G_SADDE, G_USUBE, G_SSUBE})
      .unsupported();

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
};

bool PICMidLegalizerInfo::legalizeCustom(LegalizerHelper &Helper,
                                         MachineInstr &MI) const {
  MachineRegisterInfo &MRI = MI.getMF()->getRegInfo();
  switch (MI.getOpcode()) {
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
  assert(MRI.getType(Cond.getReg()) == S1);
  auto Dest = MI.getOperand(1);
  MachineIRBuilder MIRBuilder = Helper.MIRBuilder;
  MachineInstr *DefMI = MRI.getVRegDef(Cond.getReg());
  auto CondOpc = DefMI->getOpcode();
  auto CondInstr = MIRBuilder.buildInstr(PICMid::BTFSS);
  switch (CondOpc) {
  case G_ICMP: {
    // not sure if this is correct because z is a subregister, and if this
    // should be done here get icmp type
    Register Z =
        MRI.getTargetRegisterInfo()->getRegClass(PICMid::Zc)->getRegister(0);
    CondInstr.addReg(Z);
    // build a pseudo instruction that selects through z
    break;
    }
  case G_TRUNC:
  case G_EXTRACT:
    // no code selection for trunc or extraction, instead determine bit of trunc
  default:
    return false;
    MIRBuilder.buildBr(*Dest.getMBB());
    MI.eraseFromParent();
  }
}
bool PICMidLegalizerInfo::legalizeSelect(LegalizerHelper &Helper,
                                         MachineInstr &MI) const {
  // TODO select for s8
  return false;
}

bool PICMidLegalizerInfo::legalizeICmp(LegalizerHelper &Helper,
                                       MachineInstr &MI,
                                       MachineRegisterInfo &MRI) const {
  auto [Dst, Arg1, Arg2] = MI.getFirst3Regs();
  auto [DstTy, Arg1Ty, Arg2Ty] = MI.getFirst3LLTs();
  assert(DstTy == S1);
  CmpInst::Predicate Pred =
      static_cast<CmpInst::Predicate>(MI.getOperand(1).getPredicate());
  if (Arg1Ty == S8 && Arg2Ty == S8) {
  }
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
    // this does not matter
  case CmpInst::ICMP_UGT:
  case CmpInst::ICMP_UGE:
  case CmpInst::ICMP_SGT:
  case CmpInst::ICMP_SGE:
  case CmpInst::BAD_ICMP_PREDICATE:
  default:
    return false;
  }
}