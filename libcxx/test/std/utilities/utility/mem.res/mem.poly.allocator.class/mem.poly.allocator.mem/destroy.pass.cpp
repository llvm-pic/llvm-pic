//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// TODO: Change to XFAIL once https://github.com/llvm/llvm-project/issues/40340 is fixed
// UNSUPPORTED: availability-pmr-missing

// <memory_resource>

// template <class T> class polymorphic_allocator

// template <class U>
// void polymorphic_allocator<T>::destroy(U * ptr);

#include <memory_resource>
#include <cassert>
#include <new>
#include <type_traits>

#include "test_macros.h"

int count = 0;

struct destroyable {
  destroyable() { ++count; }
  ~destroyable() { --count; }
};

int main(int, char**) {
  typedef std::pmr::polymorphic_allocator<double> A;
  {
    A a;
    ASSERT_SAME_TYPE(decltype(a.destroy((destroyable*)nullptr)), void);
  }
  {
    alignas(destroyable) char buffer[sizeof(destroyable)];
    destroyable* ptr = ::new (buffer) destroyable();
    assert(count == 1);
    A{}.destroy(ptr);
    assert(count == 0);
  }

  return 0;
}
