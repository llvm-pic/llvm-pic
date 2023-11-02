// RUN: llvm-mc -arch=picmid -show-inst %s | FileCheck %s

    addlw 69
// CHECK: <MCInst #{{[0-9]+}} ADD
// CHECK-NEXT: <MCOperand Imm:69>