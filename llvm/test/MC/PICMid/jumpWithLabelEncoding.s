; RUN: llvm-mc -arch=picmid -show-encoding %s | FileCheck %s

    nop
; CHECK: encoding: [0x00,0x00]
what:
    ADDLW 128
; CHECK: encoding: [0x80,0x3e]
    ADDWF 69, 1
; CHECK: encoding: [0xc5,0x07]
    goto what
; CHECK: encoding: [A,0b00101AAA]
; CHECK-NEXT: fixup A - offset: 0, value: what, kind: PCAbs11
