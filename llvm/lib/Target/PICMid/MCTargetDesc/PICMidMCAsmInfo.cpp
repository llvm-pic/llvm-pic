#include "PICMidMCAsmInfo.h"

llvm::PICMidMCAsmInfo::PICMidMCAsmInfo(const Triple &tt,
                                       const MCTargetOptions &options) {
  CodePointerSize = 4;
  CalleeSaveStackSlotSize = 0;
  SeparatorString = "\n";
  CommentString = ";";
  UseMotorolaIntegers = true;
  MaxInstLength = 2;
  SupportsDebugInformation = true;
}
