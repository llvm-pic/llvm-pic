; RUN: llvm-mc -arch=picmid -show-inst %s | FileCheck %s -DCURR_FILE=%s

addwf 69, 1
; CHECK: <MCInst #{{[0-9]+}} ADDWF
; CHECK-NEXT: <MCOperand Imm:69>
; CHECK-NEXT: <MCOperand Imm:1>>
ADDWF 69, 1
; CHECK: <MCInst #{{[0-9]+}} ADDWF
; CHECK-NEXT: <MCOperand Imm:69>
; CHECK-NEXT: <MCOperand Imm:1>>
andwf 69, 1
; CHECK: <MCInst #{{[0-9]+}} ANDWF
; CHECK-NEXT: <MCOperand Imm:69>
; CHECK-NEXT: <MCOperand Imm:1>>
ANDWF 69, 1
; CHECK: <MCInst #{{[0-9]+}} ANDWF
; CHECK-NEXT: <MCOperand Imm:69>
; CHECK-NEXT: <MCOperand Imm:1>>
CLRF 127 
; CHECK: <MCInst #{{[0-9]+}} CLRF
; CHECK-NEXT: <MCOperand Imm:127>>