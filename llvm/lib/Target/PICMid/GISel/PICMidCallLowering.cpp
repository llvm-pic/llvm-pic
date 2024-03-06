#include "PICMidCallLowering.h"
#include "llvm/CodeGen/GlobalISel/CallLowering.h"
#include "llvm/CodeGen/GlobalISel/MachineIRBuilder.h"

#define GET_INSTRINFO_ENUM
#include "PICMidGenInstrInfo.inc"

using namespace llvm;

bool PICMidCallLowering::lowerReturn(MachineIRBuilder &MIRBuilder,
                                     const Value *Val, ArrayRef<Register> VRegs,
                                     FunctionLoweringInfo &FLI) const {
  auto MIB = MIRBuilder.buildInstrNoInsert(PICMid::RETURN);
  if (!VRegs.empty()) {
		// TODO: Implement the none empty case, lower into physical registers.
  }
	MIRBuilder.insertInstr(MIB);
  return true;
}

bool PICMidCallLowering::lowerFormalArguments(
    MachineIRBuilder &MIRBuilder, const Function &F,
    ArrayRef<ArrayRef<Register>> VRegs, FunctionLoweringInfo &FLI) const {
		// TODO	
    // Formal arguments represent the type, size, etc. of an arg, not the values itself
    // See https://llvm.org/doxygen/classllvm_1_1Argument.html#details
  return true;
}

bool PICMidCallLowering::lowerCall(MachineIRBuilder &MIRBuilder,
                                   CallLoweringInfo &Info) const {
	// TODO	
  // Call lowering implementation fills ins with legal values
  return true;
}

/*
bool MOSCallLowering::lowerReturn(MachineIRBuilder &MIRBuilder,
                                  const Value *Val, ArrayRef<Register> VRegs,
                                  FunctionLoweringInfo &FLI) const {
  MachineFunction &MF = MIRBuilder.getMF();
  const auto &TFI = static_cast<const MOSFrameLowering &>(
      *MF.getSubtarget().getFrameLowering());
  const Function &F = MF.getFunction();

  auto Return =
      MIRBuilder.buildInstrNoInsert(TFI.isISR(MF) ? MOS::RTI : MOS::RTS);

  if (Val) {
    MachineRegisterInfo &MRI = MF.getRegInfo();
    const TargetLowering &TLI = *getTLI();
    const DataLayout &DL = MF.getDataLayout();
    LLVMContext &Ctx = Val->getContext();

    SmallVector<EVT> ValueVTs;
    ComputeValueVTs(TLI, DL, Val->getType(), ValueVTs);

    // The LLTs here are mostly redundant, except they contain information
    // missing from the VTs about whether or not the argument is a pointer.
This
    // information is added to the arg flags via adjustArgFlags below.
    SmallVector<LLT> ValueLLTs;
    computeValueLLTs(DL, *Val->getType(), ValueLLTs);
    assert(ValueVTs.size() == VRegs.size() && "Need one MVT for each VReg.");
    assert(ValueLLTs.size() == VRegs.size() && "Need one LLT for each VReg.");

    // Copy flags from the instruction definition over to the return value
    // description for TableGen compatibility layer.
    SmallVector<ArgInfo> Args;
    for (const auto &[VReg, ValueVT, ValueLLT] :
         zip(VRegs, ValueVTs, ValueLLTs)) {
      Args.emplace_back(VReg, ValueVT.getTypeForEVT(Ctx), 0);
      setArgFlags(Args.back(), AttributeList::ReturnIndex, DL, F);
      adjustArgFlags(Args.back(), ValueLLT);
    }

    // Invoke TableGen compatibility layer. This will generate copies and
stores
    // from the return value virtual register to physical and stack locations.
    MOSOutgoingReturnHandler Handler(MIRBuilder, Return, MRI);
    MOSValueAssigner Assigner(/*IsIncoming=*//*false, MRI, MF);
    if (!determineAndHandleAssignments(Handler, Assigner, Args, MIRBuilder,
                                       F.getCallingConv(), F.isVarArg()))
      return false;
  }

  // Insert the final return once the return values are in place.
  MIRBuilder.insertInstr(Return);
  return true;
}
*/