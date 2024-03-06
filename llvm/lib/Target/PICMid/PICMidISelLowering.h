#ifndef LLVM_LIB_TARGET_PICMID_PICMIDISELLOWERING_H
#define LLVM_LIB_TARGET_PICMID_PICMIDISELLOWERING_H

#include "PICMidTargetMachine.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

class PICMidTargetMachine;

class PICMidTargetLowering : public TargetLowering {

public:
};
} // namespace llvm

#endif

// class MOSSubtarget;
// class MOSTargetMachine;

// class MOSTargetLowering : public TargetLowering {
// public:
//   MOSTargetLowering(const MOSTargetMachine &TM, const MOSSubtarget &STI);

//   bool isSelectSupported(SelectSupportKind /*kind*/) const override {
//     return false;
//   }

//   // While integer division isn't "cheap", long division is not all that much
//   // slower than long multiplication, and the division->multiplication
//   // optimization this disables performs multiplciation at double the width,
//   // which is extraordinarily more expensive.
//   bool isIntDivCheap(EVT VT, AttributeList Attr) const override { return
//   true; }

//   bool areJTsAllowed(const Function *Fn) const override {
//     return !Fn->getFnAttribute("no-jump-tables").getValueAsBool();
//   }

//   MVT getRegisterType(MVT VT) const override;

//   unsigned
//   getNumRegisters(LLVMContext &Context, EVT VT,
//                   std::optional<MVT> RegisterVT = std::nullopt) const
//                   override;

//   unsigned getNumRegistersForInlineAsm(LLVMContext &Context,
//                                        EVT VT) const override;

//   ConstraintType getConstraintType(StringRef Constraint) const override;

//   MVT getRegisterTypeForCallingConv(
//       LLVMContext &Context, CallingConv::ID CC, EVT VT,
//       const ISD::ArgFlagsTy &Flags) const override;

//   unsigned
//   getNumRegistersForCallingConv(LLVMContext &Context, CallingConv::ID CC,
//                                 EVT VT,
//                                 const ISD::ArgFlagsTy &Flags) const override;

//   std::pair<unsigned, const TargetRegisterClass *>
//   getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
//                                StringRef Constraint, MVT VT) const override;

//   bool isLegalAddressingMode(const DataLayout &DL, const AddrMode &AM, Type
//   *Ty,
//                              unsigned AddrSpace,
//                              Instruction *I = nullptr) const override;

//   bool isTruncateFree(Type *SrcTy, Type *DstTy) const override;

//   bool isZExtFree(Type *SrcTy, Type *DstTy) const override;

//   bool preferNarrowTypes() const override { return true; }

//   EVT getOptimalMemOpType(const MemOp &Op,
//                           const AttributeList &FuncAttributes) const override
//                           {
//     return MVT::i8;
//   }

//   LLT getOptimalMemOpLLT(const MemOp &Op,
//                          const AttributeList &FuncAttributes) const override
//                          {
//     return LLT::scalar(8);
//   }

//   MachineBasicBlock *
//   EmitInstrWithCustomInserter(MachineInstr &MI,
//                               MachineBasicBlock *MBB) const override;
// };

// } // namespace llvm