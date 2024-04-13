#include "PICMid.h"

// #include "CommonArgs.h"

// #include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
// #include "clang/Driver/Options.h"
// #include "llvm/Support/Path.h"

using namespace llvm::opt;
using namespace clang::driver;
using namespace clang::driver::tools;
using namespace clang::driver::toolchains;

PICMidToolChain::PICMidToolChain(const Driver &D, const llvm::Triple &Triple,
         const llvm::opt::ArgList &Args)
    : ToolChain(D, Triple, Args) {
  // Look for binaries in both the installation and driver directory.
  getProgramPaths().push_back(getDriver().getInstalledDir());
  if (getDriver().getInstalledDir() != getDriver().Dir)
    getProgramPaths().push_back(getDriver().Dir);
}

void picmid::Linker::ConstructJob(Compilation &C, const JobAction &JA,
                               const InputInfo &Output,
                               const InputInfoList &Inputs, const ArgList &Args,
                               const char *LinkingOutput) const {
  // TODO: implement
}

Tool *PICMidToolChain::buildLinker() const { return new tools::picmid::Linker(*this); }