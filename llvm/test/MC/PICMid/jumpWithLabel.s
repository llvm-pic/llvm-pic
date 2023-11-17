; RUN: llvm-mc -arch=picmid -show-encoding %s | FileCheck %s

what: nop
; CHECK: encoding: [0x00,0x00]
    goto what
