#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCASMINFO_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCASMINFO_H_

#include "llvm/MC/MCAsmInfoELF.h"
#include "llvm/TargetParser/Triple.h"

namespace llvm {
class PICMidMCAsmInfo : public MCAsmInfoELF {
public:
explicit PICMidMCAsmInfo(const Triple &tt, const MCTargetOptions &options);
// unsigned getMaxInstLength(const MCSubtargetInfo *sti) const override;

};
} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDMCASMINFO_H_