; RUN: llvm-mc -arch=picmid -show-encoding %s | FileCheck %s

    ADDWF 69, 1
; CHECK: ; encoding: [0x07,0xc5]
