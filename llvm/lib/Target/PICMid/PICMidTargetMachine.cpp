//===-- PICMidTargetMachine.cpp - Instruction class definition -------*- C++
//-*-===//
//
// Part of the LLVM-PIC Project, a fork of the LLVM Project,
// by Hannes Diener, Lennart Schuster, Quang Thanh Ta, ${year),
// under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// TODO write a description
///
//===----------------------------------------------------------------------===//
#include "PICMidTargetMachine.h"
#include "GISel/PICMidInstructionSelector.h"
#include "MCTargetDesc/PICMidMCTargetDesc.h"
#include "TargetInfo/PICMidTargetInfo.h"
#include "llvm/CodeGen/GlobalISel/IRTranslator.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelect.h"
#include "llvm/CodeGen/GlobalISel/Legalizer.h"
#include "llvm/CodeGen/GlobalISel/RegBankSelect.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/InitializePasses.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Pass.h"
#include "llvm/PassRegistry.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

/// Processes a CPU name.
static StringRef getCPU(StringRef CPU) {
  return (CPU.empty() || CPU == "generic") ? "pic16f88x" : CPU;
}

// TODO: Add Data Layout
static const char *PICMidDataLayout = "E-p:8:8-i:8:8";

PICMidTargetMachine::PICMidTargetMachine(const Target &T, const Triple &TT,
                                         StringRef CPU, StringRef FS,
                                         const TargetOptions &Options,
                                         std::optional<Reloc::Model> RM,
                                         std::optional<CodeModel::Model> CM,
                                         CodeGenOptLevel OL, bool JIT)
    : LLVMTargetMachine(T, PICMidDataLayout, TT, getCPU(CPU), FS, Options,
                        Reloc::Static, CodeModel::Small, OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()) {
  initAsmInfo();
  setGlobalISel(true);
}

const TargetSubtargetInfo *
llvm::PICMidTargetMachine::getSubtargetImpl(const Function &F) const {
  Attribute CPUAttr = F.getFnAttribute("target-cpu");
  Attribute FSAttr = F.getFnAttribute("target-features");

  auto CPU = CPUAttr.isValid()
                 ? CPUAttr.getValueAsString().str()
                 : TargetCPU; // FIXME: On my MBP M3 Pro, it defaults to
                              // apple-m1. Investigate this.
  auto FS = FSAttr.isValid() ? FSAttr.getValueAsString().str()
                             : TargetFS; // See above

  auto &I = SubtargetMap[CPU + FS];
  if (!I) {
    // This needs to be done before we create a new subtarget since any
    // creation will depend on the TM and the code generation flags on the
    // function that reside in TargetOptions.
    resetTargetOptions(F);
    I = std::make_unique<PICMidSubtarget>(TargetTriple, CPU, FS, *this);
  }
  return I.get();
}

namespace {

class PICMidPassConfig : public TargetPassConfig {
public:
  PICMidPassConfig(PICMidTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  PICMidTargetMachine &getPICMidTargetMachine() const {
    return getTM<PICMidTargetMachine>();
  }

  bool addIRTranslator() override;
  bool addLegalizeMachineIR() override;
  bool addRegBankSelect() override;
  bool addGlobalInstructionSelect() override;
};

bool PICMidPassConfig::addIRTranslator() {
  addPass(new IRTranslator(getOptLevel()));
  return false;
}

bool PICMidPassConfig::addLegalizeMachineIR() {
  addPass(new Legalizer());
  return false;
}

bool PICMidPassConfig::addRegBankSelect() {
  addPass(new RegBankSelect());
  return false;
}

bool PICMidPassConfig::addGlobalInstructionSelect() {
  addPass(new InstructionSelect());
  return false;
}

} // namespace

TargetPassConfig *
llvm::PICMidTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new PICMidPassConfig(*this, PM);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePICMidTarget() {
  RegisterTargetMachine<PICMidTargetMachine> X(getThePICMidTarget());

  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializeGlobalISel(PR);
}
