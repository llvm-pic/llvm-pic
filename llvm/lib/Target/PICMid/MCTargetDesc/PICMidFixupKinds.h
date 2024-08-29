#ifndef __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDFIXUP_H_
#define __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDFIXUP_H_

#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCFixupKindInfo.h"

namespace llvm {
namespace PICMid {

enum Fixups {
  Imm1 = FirstTargetFixupKind,
  Imm3,
  Imm8,
  Addr7,
  PCAbs11,
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};

} // namespace PICMid

class PICMidFixupKinds {
public:
  const static MCFixupKindInfo &
  getFixupKindInfo(const PICMid::Fixups Kind, const MCAsmBackend *Alternative);
};

} // namespace llvm

#endif // __PROJECTS_LLVM_LLVM_PIC_LLVM_LIB_TARGET_PICMID_MCTARGETDESC_PICMIDFIXUP_H_