#include "PICMidFixupKinds.h"

namespace llvm {

const MCFixupKindInfo &
PICMidFixupKinds::getFixupKindInfo(const PICMid::Fixups Kind,
                                   const MCAsmBackend *Alternative) {
  const static MCFixupKindInfo Infos[PICMid::NumTargetFixupKinds] = {
      // This table *must* be in same the order of fixup_* kinds in
      // MOSFixupKinds.h.
      //
      // name, offset, bits, flags
      {"Imm1", 0, 1, 0},  // An 1-bit immediate value.
      {"Imm3", 0, 3, 0},  // An 3-bit immediate value.
      {"Imm8", 0, 8, 0},  // An 3-bit immediate value.
      {"Addr7", 0, 8, 0}, // An 7-bit register address.
      {"PCAbs11", 0, 11, MCFixupKindInfo::FKF_IsTarget}};

  if (Kind < static_cast<PICMid::Fixups>(FirstTargetFixupKind)) {
    assert(Alternative &&
           "Alternative PICMid backend expected, but none was given!");
    return Alternative->getFixupKindInfo(static_cast<MCFixupKind>(Kind));
  }

  assert(unsigned(Kind - FirstTargetFixupKind) <
             PICMid::Fixups::NumTargetFixupKinds &&
         "Invalid kind!");
  return Infos[Kind - FirstTargetFixupKind];
}
} // namespace llvm
