#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICBASE_PICBASETARGETMACHINE_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICBASE_PICBASETARGETMACHINE_H_

#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"

#include "PICBaseSubtarget.h"

namespace llvm {

class PICBaseTargetmachine : public LLVMTargetMachine {};
} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICBASE_PICBASETARGETMACHINE_H_