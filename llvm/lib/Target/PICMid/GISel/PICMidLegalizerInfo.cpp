#include "./PICMidLegalizerInfo.h"
#include "../PICMidSubtarget.h"
#include "llvm/CodeGen/TargetOpcodes.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Type.h"

using namespace llvm;
using namespace TargetOpcode;

PICMidLegalizerInfo::PICMidLegalizerInfo() {

  LLT S1 = LLT::scalar(1);
  LLT S8 = LLT::scalar(8);
  LLT P = LLT::pointer(0, 8);

  // Constants

  getActionDefinitionsBuilder(G_CONSTANT)
      .legalFor({S1, S8, P})
      .widenScalarToNextMultipleOf(0, 8)
      .maxScalar(0, S8)
      .unsupported();

  // Integer Extension and Truncations

  getActionDefinitionsBuilder(G_CONSTANT).legalFor({S1, S8}).unsupported();

  // Type Conversion

  // Scalar Operations

  // Integer Operations

  getActionDefinitionsBuilder({G_ADD, G_SUB})
      .legalFor({S8})
      .widenScalarToNextMultipleOf(0, 8)
      .unsupported();

  getActionDefinitionsBuilder({G_AND, G_OR})
      .legalFor({S8})
      .widenScalarToNextMultipleOf(0, 8)
      .maxScalar(0, S8)
      .unsupported();

  getActionDefinitionsBuilder(G_MUL)
      .legalFor({S8})
      .widenScalarToNextPow2(0)
      .unsupported();

  getActionDefinitionsBuilder(G_XOR)
    .legalFor({S8})
    .widenScalarToNextMultipleOf(0, 8)
    .maxScalar(0, S8) 
    .unsupported();

  getActionDefinitionsBuilder(G_ICMP)
    .legalFor({S8})
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
    .legalFor({S8})
    .unsupported();

  // Control Flow

  // Variadic Arguments

  // Other Operations

  // Optimization Hints

  // Miscellaneous
};
