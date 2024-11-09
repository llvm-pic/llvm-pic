add_llvm_component_group(PICMid)

set(LLVM_TARGET_DEFINITIONS PICMid.td)

tablegen(LLVM PICMidGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM PICMidGenAsmWriter.inc -gen-asm-writer)
# tablegen(LLVM PICMidGenCallingConv.inc -gen-callingconv)
tablegen(LLVM PICMidGenDAGISel.inc -gen-dag-isel)
# tablegen(LLVM PICMidGenDisassemblerTables.inc -gen-disassembler)
# tablegen(LLVM PICMidGenGICombiner.inc -gen-global-isel-combiner-matchtable -combiners="PICMidCombinerHelper")
tablegen(LLVM PICMidGenGlobalISel.inc -gen-global-isel)
tablegen(LLVM PICMidGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM PICMidGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM PICMidGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM PICMidGenRegisterBank.inc -gen-register-bank)
tablegen(LLVM PICMidGenRegisterInfo.inc -gen-register-info)
# tablegen(LLVM PICMidGenSearchableTables.inc -gen-searchable-tables)
tablegen(LLVM PICMidGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(PICMidCommonTableGen)

add_llvm_target(PICMidCodeGen
  PICMidAsmPrinter.cpp
  PICMidCallLowering.cpp
#   PICMidCallingConv.cpp
#   PICMidCombiner.cpp
#   PICMidCopyOpt.cpp
#   PICMidInstrCost.cpp
  PICMidFrameLowering.cpp
#   PICMidISelLowering.cpp
#   PICMidIndexIV.cpp
#   PICMidInlineAsmLowering.cpp
#   PICMidIncDecPhi.cpp
#   PICMidInsertCopies.cpp
  PICMidInstrInfo.cpp
  GISel/PICMidInstructionSelector.cpp
#   PICMidLateOptimization.cpp
  GISel/PICMidLegalizerInfo.cpp
#   PICMidLowerSelect.cpp
  PICMidInstLower.cpp
#   PICMidMachineFunctionInfo.cpp
#   PICMidMachineScheduler.cpp
#   PICMidNonReentrant.cpp
#   PICMidPostRAScavenging.cpp
  GISel/PICMidRegisterBankInfo.cpp
  PICMidRegisterInfo.cpp
#   PICMidStaticStackAlloc.cpp
  PICMidSubtarget.cpp
  PICMidTargetLowering.cpp
  PICMidTargetMachine.cpp
  PICMidTargetObjectFile.cpp
#   PICMidZeroPageAlloc.cpp

  DEPENDS
  intrinsics_gen

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  BinaryFormat
  CodeGen
  CodeGenTypes
  Core
  GlobalISel
  IPO
  InstCombine
  LTO
  MC
  PICMidDesc
  PICMidInfo
  Passes
  Scalar
  SelectionDAG
  Support
  Target
  TransformUtils

  ADD_TO_COMPONENT
  PICMid
)

add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)