#include "./PICMidLegalizerInfo.h"
#include "../PICMidSubtarget.h"
#include "llvm/CodeGen/GlobalISel/LegalizerInfo.h"
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
  LLT S16 = LLT::scalar(16);
  LLT P = LLT::pointer(0, 8);
  LLT PM = LLT::pointer(1, 16);

  // Constants

  getActionDefinitionsBuilder(G_CONSTANT)
      .legalFor({S1, S8, P})
      .widenScalarToNextMultipleOf(0, 8)
      .maxScalar(0, S8)
      .unsupported();

  // Integer Extension and Truncations

  // TODO: custom
  getActionDefinitionsBuilder(G_ANYEXT).legalFor({{S16, S8}}).unsupported();

  getActionDefinitionsBuilder(G_SEXT).custom();

  getActionDefinitionsBuilder(G_SEXT_INREG).custom();

  // TODO: custom bool
  getActionDefinitionsBuilder(G_ZEXT).maxScalar(0, S8).unsupported();

  getActionDefinitionsBuilder(G_TRUNC)
      .legalFor({{S1, S8}, {S8, S16}, {S1, S16}})
      .unsupported();

  // Type Conversion
  // TODO: Is this actually possible like this? Are those aliases?

  getActionDefinitionsBuilder(G_INTTOPTR)
      .legalFor({{P, S8}, {P, S16}})
      .unsupported();

  getActionDefinitionsBuilder(G_PTRTOINT)
      .legalFor({{S8, P}, {S16, P}})
      .unsupported();

  // Scalar Operations

  getActionDefinitionsBuilder({G_EXTRACT, G_INSERT}).lower();

  // TODO

  // Integer Operations

  getActionDefinitionsBuilder({G_ADD, G_SUB}).legalFor({S8}).unsupported();

  getActionDefinitionsBuilder({G_AND, G_OR, G_XOR})
      .legalFor({S8})
      .unsupported();

  // TODO
  getActionDefinitionsBuilder(G_MUL)
      // Other Integers
      .libcallFor({S8})
      .unsupported();

  // TODO
  getActionDefinitionsBuilder(
      {G_SDIV, G_UDIV, G_SREM, G_UREM, G_SDIVREM, G_UDIVREM})
      .libcallFor({S8});

  // TODO
  //  GSADDSAT, ...

  // TODO: check support for other typse
  getActionDefinitionsBuilder({G_LSHR, G_SHL, G_ASHR}).custom();

  getActionDefinitionsBuilder({G_ROTL, G_ROTR}).legalFor({S8}).unsupported();

  getActionDefinitionsBuilder(G_ICMP).legalFor({S8}).unsupported();

  getActionDefinitionsBuilder(G_SELECT).legalFor({S8}).unsupported();

  getActionDefinitionsBuilder(G_ICMP).legalFor({S8}).unsupported();

  // float are unsupported, as well as G_FCONSTANT
  // TODO: libcall
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
      .libcall();

  // Vector specific operations
  // Vector reductions operations

  // TODO: Should probably just be taken out in the IRTranslator

  // Memory operations

  // minimum implementation
  // W register is only 8 bit
  getActionDefinitionsBuilder({G_LOAD, G_STORE})
      .widenScalarToNextMultipleOf(0, 8)
      .maxScalar(0, S8)
      .legalFor({S8})
      .unsupported();

  // TODO: customs
  getActionDefinitionsBuilder({G_SEXTLOAD, G_ZEXTLOAD}).custom();

  getActionDefinitionsBuilder({G_MEMCPY, G_MEMMOVE, G_MEMSET, G_MEMCPY_INLINE})
      .custom();

  // Control Flow

  // TODO calling conventions

  // Variadic Arguments

  // Other Operations

  // Optimization Hints

  // Miscellaneous
};

bool PICMidLegalizerInfo::legalizeCustom(LegalizerHelper &Helper, MachineInstr &MI) const {
      
    return true;    
}

