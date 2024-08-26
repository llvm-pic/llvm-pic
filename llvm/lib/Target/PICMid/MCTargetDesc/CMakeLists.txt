add_llvm_component_library(LLVMPICMidDesc
  PICMidMCAsmBackend.cpp
  PICMidELFObjectWriter.cpp
  PICMidFixupKinds.cpp
  PICMidMCInstPrinter.cpp

  PICMidMCAsmInfo.cpp
  PICMidMCCodeEmitter.cpp
  PICMidMCObjectFileInfo.cpp
#   PICMidMCELFStreamer.cpp
  PICMidMCExpr.cpp
#   PICMidMCInstrAnalysis.cpp
  PICMidMCTargetDesc.cpp
#   PICMidTargetStreamer.cpp

  LINK_COMPONENTS
  MC
  PICMidInfo
  Support

  ADD_TO_COMPONENT
  PICMid
)

add_dependencies(LLVMPICMidDesc PICMidCommonTableGen)