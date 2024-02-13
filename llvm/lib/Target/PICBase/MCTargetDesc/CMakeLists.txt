add_llvm_component_library(LLVMPICBaseDesc
#   PICBaseMCAsmBackend.cpp
#   PICBaseELFObjectWriter.cpp
#   PICBaseFixupKinds.cpp
#   PICBaseMCInstPrinter.cpp

#   PICBaseMCAsmInfo.cpp
#   PICBaseMCCodeEmitter.cpp
#   PICBaseMCELFStreamer.cpp
#   PICBaseMCExpr.cpp
#   PICBaseMCInstrAnalysis.cpp
  PICBaseMCTargetDesc.cpp
#   PICBaseTargetStreamer.cpp

  LINK_COMPONENTS
  MC
  PICBaseInfo
  Support

  ADD_TO_COMPONENT
  PICBase
)

add_dependencies(LLVMPICBaseDesc PICBaseCommonTableGen)