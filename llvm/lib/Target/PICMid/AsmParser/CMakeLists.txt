add_llvm_component_library(LLVMPICMidAsmParser
  PICMidAsmParser.cpp
  PICMidOperand.cpp

  LINK_COMPONENTS
  MC
  MCParser
  PICMidDesc
  PICMidInfo
  Support

  ADD_TO_COMPONENT
  PICMid
)