; REQUIRED: PICMid
; RUN: rm -rf %t && split-file %s %t
; RUN-COM: clang -O0 --target=picmid -c %t/main.c -emit-llvm -o %t/main.bc
; RUN-COM: clang -O0 --target=picmid -c %t/main.c -o %t/main.o
; RUN-COM: llvm-dis %t/main.bc -o %t/main.ll
; RUN-COM: FileCheck %s --input-file=%t/main.ll

#--- main.c

int *foo = (int *)(0x30);

int main() {
    int w = 69;
    int a = w + 20;
    int b = w - 20;
    int c = w | 20;
    int d = w & 20;
    int e = w ^ 20;
    int f = ~w;

    return a;
}


