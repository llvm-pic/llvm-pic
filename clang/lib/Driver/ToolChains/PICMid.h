//===-- PICMid.h - Define ToolChain for PICMid -------*- C++ -*-===//
//
// Part of LLVM-PIC, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the clang compiler driver toolchain for PICMid.
//
//===----------------------------------------------------------------------===//

#ifndef CLANG_LIB_DRIVER_TOOLCHAINS_PICMID_H_
#define CLANG_LIB_DRIVER_TOOLCHAINS_PICMID_H_

#include "clang/Driver/Tool.h"
#include "clang/Driver/ToolChain.h"

#include "llvm/Support/Compiler.h"

namespace clang {
namespace driver {

namespace toolchains {

class LLVM_LIBRARY_VISIBILITY PICMidToolChain : public ToolChain {
public:
  PICMidToolChain(const Driver &D, const llvm::Triple &Triple,
      const llvm::opt::ArgList &Args);

protected:
  Tool *buildLinker() const override;

public:
    // Position independet Code / Executable
    bool isPICDefault() const override { return false; }
    bool isPIEDefault(const llvm::opt::ArgList &Args) const override {
        return false;
    }
    bool isPICDefaultForced() const override { return true; }

    bool HasNativeLLVMSupport() const override { return true; }
    bool IsIntegratedAssemblerDefault() const override { return true; }

    const char *getDefaultLinker() const override { return "ld.lld"; }
    RuntimeLibType GetDefaultRuntimeLibType() const override {
        // for options see ToolChain.h; other option would be libgcc
        return ToolChain::RLT_CompilerRT;
    }

    bool SupportsProfiling() const override { return false; }

    void AddClangSystemIncludeArgs(const llvm::opt::ArgList &DriverArgs,
                        llvm::opt::ArgStringList &CC1Args) const override;

    void addClangTargetOptions(const llvm::opt::ArgList &DriverArgs,
                        llvm::opt::ArgStringList &CC1Args,
                        Action::OffloadKind DeviceOffloadKind) const override;


    virtual bool isThreadModelSupported(const StringRef Model) const { return false; };

    virtual std::string getThreadModel() const { return "single"; }

};

} // namespace toolchains

namespace tools {
namespace picmid {

class LLVM_LIBRARY_VISIBILITY Linker : public Tool {
public:
  Linker(const ToolChain &TC) : Tool("PICMid::Linker", "ld.lld", TC) {}
  bool isLinkJob() const override { return true; }
  bool hasIntegratedCPP() const override { return false; }
  void ConstructJob(Compilation &C, const JobAction &JA,
    const InputInfo &Output, const InputInfoList &Inputs,
    const llvm::opt::ArgList &TCArgs,
    const char *LinkingOutput) const override;
};

} // namespace picmid
} // namespace tools

} // namespace driver
} // namespace clang

#endif // not CLANG_LIB_DRIVER_TOOLCHAINS_PICMID_H_

