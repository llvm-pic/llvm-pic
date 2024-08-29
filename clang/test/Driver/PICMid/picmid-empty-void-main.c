// REQUIRED: PICMid
//  RUN: rm -rf %t && split-file %s %t
//  RUN: %clang -nostartfiles -T%t/link.ld %t/void.c -o %t/a

#--- void.c

void main(void) {
}


#--- link.ld
