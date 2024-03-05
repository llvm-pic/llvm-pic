; REQUIRED: PICMid
; RUN: rm -rf %t && split-file %s %t
; RUN: llvm-mc -filetype=obj -triple=picmid %t/startup.s -o %t/startup.o
; RUN: llc -filetype=obj -mtriple=picmid --start-before=instruction-select %t/main.mir -o %t/main.o
; RUN: ld.lld -T %t/link.ld %t/startup.o %t/main.o -o %t/a
; RUN: llvm-objcopy -O ihex %t/a %t/picmid-basic-mir.hex
; RUN: FileCheck %s --check-prefix=CHECK1 --input-file=%t/picmid-basic-mir.hex

; CHECK1:       :020000000628D0
; CHECK1-NEXT:  :10000A00000008200728142016201E202020222065
; CHECK1-NEXT:  :10001A002420262028202A202C202E2008002A3EB0
; CHECK1-NEXT:  :10002A000800F8007008F1007808F1077108F00775
; CHECK1-NEXT:  :10003A0008002A3C0800F00208002A390800F005E6
; CHECK1-NEXT:  :10004A0008002A380800F00408002A3A0800F006D6
; CHECK1-NEXT:  :06005A000800F009080097
; CHECK1-NEXT:  :040000030000000CED
; CHECK1-NEXT:  :00000001FF


#--- link.ld
MEMORY {
    prog_mem : ORIGIN = 0x0000, LENGTH = 0x2000
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
    call main

loop:
    goto loop

#--- main.mir
---
name: main
legalized: true
regBankSelected: true
tracksRegLiveness: true
body: |
  bb.0.entry:
    G_CALL &add_imm
    G_CALL &add_f
    G_CALL &sub_imm
    G_CALL &sub_f
    G_CALL &and_imm
    G_CALL &and_f
    G_CALL &ior_imm
    G_CALL &ior_f
    G_CALL &xor_imm
    G_CALL &xor_f
    G_CALL &comf_f
    G_RETURN
---
name: add_imm
legalized: true
regBankSelected: true
tracksRegLiveness: true
body: |
  bb.0.entry:
    liveins: $w, $r0
    %0:any(s8) = COPY $w
    %1:any(s8) = G_CONSTANT i8 42
    %2:any(s8) = G_ADD %0, %1
    G_RETURN implicit %2
---
name: add_f
legalized: true
regBankSelected: true
tracksRegLiveness: true
body: |
  bb.0.entry:
    liveins: $w, $r0
    %0:any(s8) = COPY $w
    %1:any(s8) = COPY $r0
    %2:any(s8) = G_ADD %0, %1
    %3:any(s8) = G_ADD %2, %1
    G_RETURN implicit %3
---
name: sub_imm
legalized: true
regBankSelected: true
tracksRegLiveness: true
body: |
  bb.0.entry:
    liveins: $w, $r0
    %0:any(s8) = COPY $w
    %1:any(s8) = G_CONSTANT i8 42
    %2:any(s8) = G_SUB %1, %0
    G_RETURN implicit %2
---
name: sub_f
legalized: true
regBankSelected: true
tracksRegLiveness: true
body: |
  bb.0.entry:
    liveins: $w, $r0
    %0:any(s8) = COPY $w
    %1:any(s8) = COPY $r0
    %2:any(s8) = G_SUB %1, %0
    G_RETURN implicit %2
---
name: and_imm
legalized: true
regBankSelected: true
tracksRegLiveness: true
body: |
  bb.0.entry:
    liveins: $w
    %0:any(s8) = COPY $w
    %1:any(s8) = G_CONSTANT i8 42
    %2:any(s8) = G_AND %0, %1
    G_RETURN implicit %2

---
name: and_f
legalized: true
regBankSelected: true
tracksRegLiveness: true
body: |
  bb.0.entry:
    liveins: $w, $r0
    %0:any(s8) = COPY $w
    %1:any(s8) = COPY $r0
    %2:any(s8) = G_AND %0, %1
    G_RETURN implicit %2

---
name: ior_imm
legalized: true
regBankSelected: true
tracksRegLiveness: true
body: |
  bb.0.entry:
    liveins: $w
    %0:any(s8) = COPY $w
    %1:any(s8) = G_CONSTANT i8 42
    %2:any(s8) = G_OR %0, %1
    G_RETURN implicit %2

---
name: ior_f
legalized: true
regBankSelected: true
tracksRegLiveness: true
body: |
  bb.0.entry:
    liveins: $w, $r0
    %0:any(s8) = COPY $w
    %1:any(s8) = COPY $r0
    %2:any(s8) = G_OR %0, %1
    G_RETURN implicit %2

---
name: xor_imm
legalized: true
regBankSelected: true
tracksRegLiveness: true
body: |
  bb.0.entry:
    liveins: $w
    %0:any(s8) = COPY $w
    %1:any(s8) = G_CONSTANT i8 42
    %2:any(s8) = G_XOR %0, %1
    G_RETURN implicit %2

---
name: xor_f
legalized: true
regBankSelected: true
tracksRegLiveness: true
body: |
  bb.0.entry:
    liveins: $w, $r0
    %0:any(s8) = COPY $w
    %1:any(s8) = COPY $r0
    %2:any(s8) = G_XOR %0, %1
    G_RETURN implicit %2

---
name: comf_f
legalized: true
regBankSelected: true
tracksRegLiveness: true
body: |
  bb.0.entry:
    liveins: $w, $r0
    %0:any(s8) = COPY $r0
    %1:any(s8) = G_CONSTANT i8 -1
    %2:any(s8) = G_XOR %0, %1
    G_RETURN implicit %2