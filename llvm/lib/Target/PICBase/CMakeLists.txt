add_llvm_component_group(PICBase)

set(LLVM_TARGET_DEFINITIONS PICBase.td)

# tablegen(LLVM PICBaseGenAsmMatcher.inc -gen-asm-matcher)
# tablegen(LLVM PICBaseGenAsmWriter.inc -gen-asm-writer)
# tablegen(LLVM PICBaseGenCallingConv.inc -gen-callingconv)
# tablegen(LLVM PICBaseGenDAGISel.inc -gen-dag-isel)
# tablegen(LLVM PICBaseGenDisassemblerTables.inc -gen-disassembler)
# tablegen(LLVM PICBaseGenGICombiner.inc -gen-global-isel-combiner-matchtable -combiners="PICBaseCombinerHelper")
# tablegen(LLVM PICBaseGenGlobalISel.inc -gen-global-isel)
tablegen(LLVM PICBaseGenInstrInfo.inc -gen-instr-info)
# tablegen(LLVM PICBaseGenMCCodeEmitter.inc -gen-emitter)
# tablegen(LLVM PICBaseGenMCPseudoLowering.inc -gen-pseudo-lowering)
# tablegen(LLVM PICBaseGenRegisterBank.inc -gen-register-bank)
tablegen(LLVM PICBaseGenRegisterInfo.inc -gen-register-info)
# tablegen(LLVM PICBaseGenSearchableTables.inc -gen-searchable-tables)
tablegen(LLVM PICBaseGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(PICBaseCommonTableGen)

add_llvm_target(PICBaseCodeGen
#   PICBaseAsmPrinter.cpp
#   PICBaseCallLowering.cpp
#   PICBaseCallingConv.cpp
#   PICBaseCombiner.cpp
#   PICBaseCopyOpt.cpp
#   PICBaseInstrCost.cpp
#   PICBaseFrameLowering.cpp
#   PICBaseISelLowering.cpp
#   PICBaseIndexIV.cpp
#   PICBaseInlineAsmLowering.cpp
#   PICBaseIncDecPhi.cpp
#   PICBaseInsertCopies.cpp
#   PICBaseInstrInfo.cpp
#   PICBaseInstructionSelector.cpp
#   PICBaseLateOptimization.cpp
#   GISel/PICBaseLegalizerInfo.cpp
#   PICBaseLowerSelect.cpp
#   PICBaseMCInstLower.cpp
#   PICBaseMachineFunctionInfo.cpp
#   PICBaseMachineScheduler.cpp
#   PICBaseNonReentrant.cpp
#   PICBasePostRAScavenging.cpp
#   PICBaseRegisterBankInfo.cpp
#   PICBaseRegisterInfo.cpp
#   PICBaseStaticStackAlloc.cpp
  PICBaseSubtarget.cpp
  PICBaseTargetMachine.cpp
#   PICBaseTargetObjectFile.cpp
#   PICBaseZeroPageAlloc.cpp

  DEPENDS
  intrinsics_gen

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  BinaryFormat
  CodeGen
  Core
  GlobalISel
  IPO
  InstCombine
  LTO
  MC
  PICBaseDesc
  PICBaseInfo
  Passes
  Scalar
  SelectionDAG
  Support
  Target
  TransformUtils

  ADD_TO_COMPONENT
  PICBase
)

# add_subdirectory(AsmParser)
# add_subdirectory(Disassembler)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)