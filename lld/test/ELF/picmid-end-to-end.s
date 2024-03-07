; REQUIRED: PICMid
; RUN: rm -rf %t && split-file %s %t
; RUN: clang -O0 --target=picmid -c %t/main.c -emit-llvm -o %t/main.bc
; RUN-COM: clang -Os --target=picmid -c %t/main.c -o %t/main.o
; RUN: opt -passes=mem2reg %t/main.bc -o %t/main-opt.bc
; RUN: llvm-dis %t/main-opt.bc -o %t/main.ll
; RUN-COM: FileCheck %s --input-file=%t/main.ll

#--- main.c


int main() {
    int w = 69;
    int a = w + 20;
    int b = w - 20;
    int c = w | 20;
    int d = w & 20;
    int e = w ^ 20;
    int f = ~w;

    *((int *) 0x30) = 69;

    return a;
}


