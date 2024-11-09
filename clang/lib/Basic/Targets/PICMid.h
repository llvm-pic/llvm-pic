#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_PICMID_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_PICMID_H
#include "clang/AST/Type.h"
#include "clang/Basic/Builtins.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/Support/Compiler.h"
#include "llvm/TargetParser/Triple.h"
namespace clang {
namespace targets {
class LLVM_LIBRARY_VISIBILITY PICMidTargetInfo : public TargetInfo {
  static const char *const GCCRegNames[];

public:
  PICMidTargetInfo(const llvm::Triple &Triple, const TargetOptions &)
      : TargetInfo(Triple) {
    TLSSupported = false;
    IntWidth = 8;
    IntAlign = 8;
    LongWidth = 16;
    LongLongWidth = 32;
    LongAlign = LongLongAlign = 16;
    //FloatWidth = 8;
    //FloatAlign = 8;
    //DoubleWidth = LongDoubleWidth = 8;
    //DoubleAlign = LongDoubleAlign = 8;
    PointerWidth = 8;
    PointerAlign = 8;
    SuitableAlign = 8;
    SizeType = UnsignedInt;
    //IntMaxType = SignedLongLong;
    //IntPtrType = SignedInt;
    //PtrDiffType = SignedInt;
    //SigAtomicType = SignedLong;
    MaxAtomicPromoteWidth = 8;
    resetDataLayout("E-p:8:8-i:8:8");
  }
  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override;

  ArrayRef<Builtin::Info> getTargetBuiltins() const override {
    return std::nullopt;
  }

  bool allowsLargerPreferedTypeAlignment() const override { return false; }

  bool hasFeature(StringRef Feature) const override {
    return Feature == "picmid";
  }

  ArrayRef<const char *> getGCCRegNames() const override;

  ArrayRef<TargetInfo::GCCRegAlias> getGCCRegAliases() const override {
    // Make r0 - r3 be recognized by llc (f.e., in clobber list)
    static const TargetInfo::GCCRegAlias GCCRegAliases[] = {
        {{"r0"}, "0"},
        {{"r1"}, "1"},
        {{"r2"}, "2"},
        {{"r3"}, ""},
    };
    return llvm::ArrayRef(GCCRegAliases);
  }

  bool validateAsmConstraint(const char *&Name,
                             TargetInfo::ConstraintInfo &info) const override {
    // No target constraints for now.
    return false;
  }

  std::string_view getClobbers() const override {
    // FIXME: Is this really right?
    return "";
  }

  BuiltinVaListKind getBuiltinVaListKind() const override {
    // FIXME: implement
    return TargetInfo::CharPtrBuiltinVaList;
  }
};
} // namespace targets
} // namespace clang
#endif
