//===-- PICMidMCObjectFileInfo.h - Define TargetMachine for PICMid -*- C++
//-*-===//
//
// Part of LLVM-PIC, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===-------------------------------------------------------------------------===//
//
// This file defines general information about the object file produced by
// LLVM's PICMid compiler. Most notably, the PICMidMCObjectFileInfo class
// defines the text section alignment. I.e., the address of the beginning
// of each text section in bytes must be a multiple of 2.
//
//===-------------------------------------------------------------------------===//
#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCOBJECTFILEINFO_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCOBJECTFILEINFO_H_

#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"

namespace llvm {

class PICMidMCObjectFileInfo : public MCObjectFileInfo {
public:
  static unsigned getTextSectionAlignment(const MCSubtargetInfo &STI);
  unsigned getTextSectionAlignment() const override;
};

} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCOBJECTFILEINFO_H_