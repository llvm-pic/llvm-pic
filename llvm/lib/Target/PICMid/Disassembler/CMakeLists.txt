add_llvm_component_library(LLVMPICMidDisassembler
  PICMidDisassembler.cpp

  LINK_COMPONENTS
  MC
  MCDisassembler
  PICMidDesc
  PICMidInfo
  Support

  ADD_TO_COMPONENT
  PICMid
)