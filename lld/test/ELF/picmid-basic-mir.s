; REQUIRED: PICMid
; RUN: rm -rf %t && split-file %s %t
; RUN: llvm-mc -filetype=obj -triple=picmid %t/startup.s -o %t/startup.o
; RUN: llc -filetype=obj -mtriple=picmid --start-before=instruction-select %t/add_imm.mir -o %t/add_imm.o
; RUN: ld.lld -T %t/link.ld %t/startup.o %t/add_imm.o -o %t/a
; RUN: llvm-objcopy -O ihex %t/a %t/a.hex
; RUN: FileCheck %s --check-prefix=CHECK1 --input-file=%t/a.hex

; CHECK1:       :020000000528D1
; CHECK1-NEXT:  :10000A00082006280000F8007008F1007808F107B7
; CHECK1-NEXT:  :06001A007108F007080068
; CHECK1-NEXT:  :040000030000000AEF
; CHECK1-NEXT:  :00000001FF

#--- link.ld
MEMORY {
    prog_mem : ORIGIN = 0x0000, LENGTH = 0x4000
    config_mem : ORIGIN = 0x4000, LENGTH = 0x4000
}

SECTIONS {
    .reset_vector 0x0000 : {
        KEEP(*(.reset_vector))
    } > prog_mem

    .text 0x000A : { 
        *(.text) 
        *(.data)
        *(.bss)
    } > prog_mem

    .config1 0x4014 : { 
        KEEP(*(.config1))
    } > config_mem
    .config2 0x4016 : { 
        KEEP(*(.config2)) 
    } > config_mem
}

#--- startup.s
    .globl _start

    .section .reset_vector,"ax",@progbits
    goto _start

    .text
_start:
    call add_imm

loop:
    goto loop

#--- add_imm.mir
---
name: add_imm
legalized: true
regBankSelected: true
tracksRegLiveness: true
body: |
  bb.0.entry:
    liveins: $w, $r0, $r1, $r8
    %0:any(s8) = COPY $w
    %1:any(s8) = COPY $r0
    %2:any(s8) = G_ADD %0, %1
    %3:any(s8) = G_ADD %2, %1
    G_RETURN implicit %3
