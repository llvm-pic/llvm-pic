//===-- PICMidTargetMachine.h - Define TargetMachine for PICMid -------*- C++ -*-===//
//
// Part of LLVM-PIC, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the PICMid specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_PICMID_TARGET_MACHINE_H
#define LLVM_PICMID_TARGET_MACHINE_H

#include "PICMidSubtarget.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

/// A generic MOS implementation.
class PICMidTargetMachine : public LLVMTargetMachine {
public:
  PICMidTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                      StringRef FS, const TargetOptions &Options,
                      std::optional<Reloc::Model> RM,
                      std::optional<CodeModel::Model> CM, CodeGenOpt::Level OL,
                      bool JIT);

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  const TargetSubtargetInfo *getSubtargetImpl(const Function &F) const override;

private:
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  mutable StringMap<std::unique_ptr<PICMidSubtarget>> SubtargetMap;
};

} // end namespace llvm

#endif // LLVM_PICMid_TARGET_MACHINE_H
