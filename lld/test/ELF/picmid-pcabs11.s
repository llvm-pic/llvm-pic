; REQUIRED: PICMid
; RUN: rm -rf %t && split-file %s %t
; RUN: llvm-mc -filetype=obj -triple=picmid %t/a.s -o %t/a.o
; RUN: ld.lld -T %t/link.ld %t/a.o -o %t/a
; RUN: llvm-objcopy -O ihex %t/a %t/a.hex
; RUN: FileCheck %s --check-prefix=CHECK1 --input-file=%t/a.hex 


; CHECK1:        :1000000000000000000000000000000000000000F0
; CHECK1-NEXT:   :1000100000000000000000000000000000000000E0
; CHECK1-NEXT:   :1000200000000000000000000000000000000000D0
; CHECK1-NEXT:   :1000300000000000000000000000000000000000C0
; CHECK1-NEXT:   :1000400000000000000000000000000000000000B0
; CHECK1-NEXT:   :1000500000000000000000000000000000000000A0
; CHECK1-NEXT:   :100060000000000000000000000000000000000090
; CHECK1-NEXT:   :100070000000000000000000000000000000000080
; CHECK1-NEXT:   :100080000000000000000000000000000000000070
; CHECK1-NEXT:   :100090000000000000000000000000000000000060
; CHECK1-NEXT:   :1000A0000000000000000000000000000000000050
; CHECK1-NEXT:   :1000B0000000000000000000000000000000000040
; CHECK1-NEXT:   :1000C0000000000000000000000000000000000030
; CHECK1-NEXT:   :1000D0000000000000000000000000000000000020
; CHECK1-NEXT:   :1000E0000000000000000000000000000000000010
; CHECK1-NEXT:   :1000F0000000000000000000000000000000000000
; CHECK1-NEXT:   :1001000000000000000000000000000000000000EF
; CHECK1-NEXT:   :1001100000000000000000000000000000000000DF
; CHECK1-NEXT:   :1001200000000000000000000000000000000000CF
; CHECK1-NEXT:   :1001300000000000000000000000000000000000BF
; CHECK1-NEXT:   :1001400000000000000000000000000000000000AF
; CHECK1-NEXT:   :10015000000000000000000000000000000000009F
; CHECK1-NEXT:   :10016000000000000000000000000000000000008F
; CHECK1-NEXT:   :10017000000000000000000000000000000000007F
; CHECK1-NEXT:   :10018000000000000000000000000000000000006F
; CHECK1-NEXT:   :10019000000000000000000000000000000000005F
; CHECK1-NEXT:   :1001A000000000000000000000000000000000004F
; CHECK1-NEXT:   :1001B000000000000000000000000000000000003F
; CHECK1-NEXT:   :1001C000000000000000000000000000000000002F
; CHECK1-NEXT:   :1001D000000000000000000000000000000000001F
; CHECK1-NEXT:   :1001E000000000000000000000000000000000000F
; CHECK1-NEXT:   :1001F00000000000000000000000000000000000FF
; CHECK1-NEXT:   :1002000000000000000000000000000000000000EE
; CHECK1-NEXT:   :1002100000000000000000000000000000000000DE
; CHECK1-NEXT:   :1002200000000000000000000000000000000000CE
; CHECK1-NEXT:   :1002300000000000000000000000000000000000BE
; CHECK1-NEXT:   :08024000803EC507202920299A
; CHECK1-NEXT:   :00000001FF


#--- link.ld

SECTIONS
{
. = 0x0;
.text : { *(.text) }
. = 0x8000000;
.data : { *(.data) }
.bss : { *(.bss) }
}

#--- a.s

    .globl _start;
    .text
_start:
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
what: ; Label at address 0x120
    ADDLW 128
    ADDWF 69, 1
    GOTO what
    GOTO what