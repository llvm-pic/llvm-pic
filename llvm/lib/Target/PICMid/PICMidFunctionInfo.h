#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDFUNCTIONINFO_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDFUNCTIONINFO_H_

#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {
class PICMidSubtarget;

struct PICMidFunctionInfo : public MachineFunctionInfo {
  PICMidFunctionInfo(const Function &F, const PICMidSubtarget *STI) {}

  int VarArgsStackIndex = -1;
  const GlobalValue *StaticStackValue = nullptr;
  const GlobalValue *ZeroPageStackValue = nullptr;
  DenseMap<Register, size_t> CSRZPOffsets;
};
} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDFUNCTIONINFO_H_
