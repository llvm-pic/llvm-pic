; RUN: llvm-mc -arch=picmid -show-encoding %s | FileCheck %s

    ADDWF 69, 1
; CHECK: encoding: [0xc5,0x07]
