; RUN: llvm-mc -arch=picmid -show-encoding %s | FileCheck %s

what: nop
; CHECK: encoding: [0x00,0x00]
    ADDLW 69
; CHECK: encoding: [0x45,0x3e]
    ; goto what
