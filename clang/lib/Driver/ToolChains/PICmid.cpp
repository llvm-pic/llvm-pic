//===-- PICMid.cpp - Define ToolChain for PICMid -------*- C++ -*-===//
//
// Part of LLVM-PICMid, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

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

void PICMidToolChain::addClangTargetOptions(const ArgList &DriverArgs,
                                ArgStringList &CC1Args,
                                Action::OffloadKind) const {
  CC1Args.push_back("-nostdsysteminc");
  // set to freestanding environment (main return type void)
  CC1Args.push_back("-ffreestanding");
  // set language Standard to C17
  CC1Args.push_back("-std=c17");
  // omit frame pointers because we don't use frame pointers for our simulated stack
  CC1Args.push_back("-mframe-pointer=none");
  // CC1Args.push_back("-stack-protector-buffer-size 0");
}

void picmid::Linker::ConstructJob(Compilation &C, const JobAction &JA,
                               const InputInfo &Output,
                               const InputInfoList &Inputs, const ArgList &Args,
                               const char *LinkingOutput) const {
  // TODO: implement
}

Tool *PICMidToolChain::buildLinker() const { return new tools::picmid::Linker(*this); }