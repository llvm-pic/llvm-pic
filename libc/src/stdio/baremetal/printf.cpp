//===-- Implementation of printf for baremetal ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/stdio/printf.h"
#include "src/__support/arg_list.h"
#include "src/stdio/printf_core/core_structs.h"
#include "src/stdio/printf_core/printf_main.h"
#include "src/stdio/printf_core/writer.h"

#include <stdarg.h>

// TODO(https://github.com/llvm/llvm-project/issues/94685) unify baremetal hooks

// This is intended to be provided by the vendor.
extern "C" size_t __llvm_libc_raw_write(const char *s, size_t size);

namespace LIBC_NAMESPACE {

namespace {

LIBC_INLINE int raw_write_hook(cpp::string_view new_str, void *) {
  size_t written = __llvm_libc_raw_write(new_str.data(), new_str.size());
  if (written != new_str.size())
    return printf_core::FILE_WRITE_ERROR;
  return printf_core::WRITE_OK;
}

} // namespace

LLVM_LIBC_FUNCTION(int, printf, (const char *__restrict format, ...)) {
  va_list vlist;
  va_start(vlist, format);
  internal::ArgList args(vlist); // This holder class allows for easier copying
                                 // and pointer semantics, as well as handling
                                 // destruction automatically.
  va_end(vlist);
  constexpr size_t BUFF_SIZE = 1024;
  char buffer[BUFF_SIZE];

  printf_core::WriteBuffer wb(buffer, BUFF_SIZE, &raw_write_hook, nullptr);
  printf_core::Writer writer(&wb);

  int retval = printf_core::printf_main(&writer, format, args);

  int flushval = wb.overflow_write("");
  if (flushval != printf_core::WRITE_OK)
    retval = flushval;

  return retval;
}

} // namespace LIBC_NAMESPACE