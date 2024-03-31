#include "PICMidLegalizerInfo.h"
#include "MCTargetDesc/PICMidFixupKinds.h"
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
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Type.h"

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

  getActionDefinitionsBuilder({G_TRUNC}).maxScalar(0, S8).unsupported();

  // Type Conversion

  // INFO this should only effect memory operations is therefor correct
  getActionDefinitionsBuilder({G_INTTOPTR}).legalFor({P, S8}).unsupported();

  getActionDefinitionsBuilder({G_PTRTOINT}).legalFor({S8, P}).unsupported();

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

  getActionDefinitionsBuilder({G_SELECT}).legalFor({S1});

  getActionDefinitionsBuilder({G_SHL, G_LSHR}).legalFor({S8}).unsupported();

  getActionDefinitionsBuilder(G_ICMP).legalFor({S8}).unsupported();

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

  // TODO Cross Bank Load and Store legalisation
  getActionDefinitionsBuilder({G_LOAD, G_STORE}).legalFor({S8}).unsupported();

  // Fence will never appear as PIC is single thread
  getActionDefinitionsBuilder({G_FENCE}).unsupported();

  // Control Flow

  getActionDefinitionsBuilder({G_PHI}).alwaysLegal();

  getActionDefinitionsBuilder(G_BR).alwaysLegal();

  // Variadic Arguments

  // Other Operations

  // Optimization Hints

  // Miscellaneous
};

bool PICMidLegalizerInfo::legalizeCustom(LegalizerHelper &Helper,
                                         MachineInstr &MI) const {
  switch (MI.getOpcode()) {
  default:
    return false;
  }
}
