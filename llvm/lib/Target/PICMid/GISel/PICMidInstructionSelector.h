//===-- PICMidInstructionSelector.h - Last GlobalISel pass ------*- cpp -*-===//
//
// Part of LLVM-PIC, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains a function which creates a GlobalISel instruction
// selector for PIC mid-range devices. This function is called by
// PICMidTargetMachine's PassConfig.
//
//===----------------------------------------------------------------------===//
#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDINSTRUCTIONSELECTOR_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDINSTRUCTIONSELECTOR_H_

// #include "PICMidRegisterBankInfo.h"
#include "PICMidRegisterBankInfo.h"
#include "PICMidSubtarget.h"
#include "PICMidTargetMachine.h"
#include "llvm/CodeGen/GlobalISel/InstructionSelector.h"

namespace llvm {
InstructionSelector *
createPICMidInstructionSelector(const PICMidTargetMachine &TM,
                                PICMidSubtarget &STI,
                                PICMidRegisterBankInfo &RBI);
} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_PICMIDINSTRUCTIONSELECTOR_H_