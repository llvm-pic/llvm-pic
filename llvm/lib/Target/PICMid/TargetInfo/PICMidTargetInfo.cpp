//===-- MOSTargetInfo.cpp - MOS Target Implementation ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/PICMidTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

namespace llvm {

Target &getThePICMidTarget() {
  static Target ThePICMidTarget;
  return ThePICMidTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void
LLVMInitializePICMidTargetInfo() { // NOLINT
  RegisterTarget<Triple::picmid> X(llvm::getThePICMidTarget(), "picmid",
                                   "PIC(c) Mid-range core devices", "PICMid");
}

} // namespace llvm
