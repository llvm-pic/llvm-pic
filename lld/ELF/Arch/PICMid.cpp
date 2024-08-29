#include "InputFiles.h"
#include "Symbols.h"
#include "Target.h"
#include "lld/Common/ErrorHandler.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/Object/ELF.h"
#include "llvm/Support/Endian.h"

using namespace llvm;
using namespace llvm::object;
using namespace llvm::support::endian;
using namespace llvm::ELF;

namespace lld {

namespace elf {

namespace {
class PICMid final : public TargetInfo {
public:
  PICMid();
  //   uint32_t calcEFlags() const override; // TODO: Handle
  RelExpr getRelExpr(RelType type, const Symbol &s,
                     const uint8_t *loc) const override;

  void relocate(uint8_t *loc, const Relocation &rel,
                uint64_t val) const override;
};

} // namespace

PICMid::PICMid() {
  defaultMaxPageSize = 4;
  defaultCommonPageSize = 4;
}

RelExpr PICMid::getRelExpr(RelType type, const Symbol &s,
                           const uint8_t *loc) const {
  switch (type) {
  default:
    return R_ABS;
  }
}

void PICMid::relocate(uint8_t *loc, const Relocation &rel, uint64_t val) const {
  uint64_t pcAddr;
  switch (rel.type) {
  case R_PICMID_IMM8:
    checkIntUInt(loc, val, 8, rel);
    *loc = static_cast<unsigned char>(val);
    break;
  case R_PICMID_ADDR7:
    checkUInt(loc, val, 7, rel);
    *loc &= ~0x7F;
    *loc |= static_cast<unsigned char>(val);
    break;
  case R_PICMID_PCABS11:
    assert(val % 2 == 0 &&
           "Program memory address in bytes must be evenly divisible by 2");
    pcAddr = val / 2;
    checkUInt(loc, pcAddr, 11, rel);
    write16le(loc, (read16le(loc) & ~0x07FF) | (pcAddr & 0x07FF));
    break;
  default:
  case R_PICMID_IMM1:
  case R_PICMID_IMM3:
    error(getErrorLocation(loc) + "unrecognized relocation " +
          toString(rel.type));
    break;
  }
}

TargetInfo *getPICMidTargetInfo() {
  static PICMid target;
  return &target;
}

} // namespace elf

} // namespace lld
