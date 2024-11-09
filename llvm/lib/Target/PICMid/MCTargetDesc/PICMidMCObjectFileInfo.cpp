#include "PICMidMCObjectFileInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCSubtargetInfo.h"

using namespace llvm;

unsigned llvm::PICMidMCObjectFileInfo::getTextSectionAlignment(
    const MCSubtargetInfo &STI) {
  return 2;
}

unsigned llvm::PICMidMCObjectFileInfo::getTextSectionAlignment() const {
  return getTextSectionAlignment(*getContext().getSubtargetInfo());
}
