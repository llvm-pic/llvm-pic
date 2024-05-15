; REQUIRED: PICMid
; RUN: rm -rf %t && split-file %s %t
; RUN: llvm-mc -filetype=obj -triple=picmid %t/a.s -o %t/a.o
; RUN: llvm-mc -filetype=obj -triple=picmid %t/config.s -o %t/config.o
; RUN: ld.lld -T %t/link.ld %t/a.o %t/config.o -o %t/a
; RUN: llvm-objcopy -O ihex %t/a %t/a.hex
; RUN: FileCheck %s --check-prefix=CHECK1 --input-file=%t/a.hex 

; CHECK1:       :020000000528D1
; CHECK1-NEXT:  :10000A0083120313870183160317880183160313C8
; CHECK1-NEXT:  :10001A000C308700831203130714871400000000B2
; CHECK1-NEXT:  :02002A00142898
; CHECK1-NEXT:  :024014000000AA
; CHECK1-NEXT:  :024016000000A8
; CHECK1-NEXT:  :040000030000000AEF
; CHECK1-NEXT:  :00000001FF

; TODO: Document "ax"
; TODO: Use FileCheck

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

#--- a.s

    .globl _start

    .section .reset_vector,"ax",@progbits
    goto _start

    .text
_start:
;    BANKSEL PORTA
;    CLRF    PORTA
;    BANKSEL ANSEL
;    CLRF    ANSEL
;    BANKSEL TRISA
;    MOVLW   0Ch
;    MOVWF   TRISA
;    # STATUS @ 0x03
;    # PORTA @ 0x05
;    # PORTC @ 0x07
;    # ANSEL @ 0x188
;    # TRISA @ 0x85
;    # PORTC @ 0x87
    BCF     0x03, 5 ; BANKSEL PORTC
    BCF     0x03, 6
    CLRF    0x07    ; CLRF PORTC
    BSF     0x03, 5 ; BANKSEL ANSEL
    BSF     0x03, 6
    CLRF    0x08    ; CLRF ANSEL
    BSF     0x03, 5 ; BANKSEL TRISC
    BCF     0x03, 6
    MOVLW   0x0C    ; MOVLW 0Ch
    MOVWF   0x07
    BCF     0x03, 5 ; BANKSEL PORTC
    BCF     0x03, 6
    BSF     0x07, 0  ; CLRF PORTC
    BSF     0x07, 1  ; CLRF PORTC

    nop
loop:
    nop
    goto    loop


#--- config.s

    .section .config1,"ax",@progbits
.byte   0x00, 0x00

    .section .config2,"ax",@progbits
.byte   0x00, 0x00
