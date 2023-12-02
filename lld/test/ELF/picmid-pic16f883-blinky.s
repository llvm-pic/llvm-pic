; REQUIRED: PICMid
; RUN: rm -rf %t && split-file %s %t
; RUN: llvm-mc -filetype=obj -triple=picmid %t/a.s -o %t/a.o
; RUN: llvm-mc -filetype=obj -triple=picmid %t/config.s -o %t/config.o
; RUN: ld.lld -T %t/link.ld %t/a.o %t/config.o -o %t/a
; RUN: llvm-objcopy -O ihex %t/a %t/a.hex

; TODO: Document "ax"
; TODO: Use FileCheck
; FIXME: The number of bytes stored inside the resulting intel hex file
;           is always a multiple of 4. Should be 2. 

#--- link.ld

SECTIONS
{
.reset_vector 0x0000 : {
    KEEP(*(.reset_vector))
}

. = 0x0010;
.text : { *(.text) }
.data : { *(.data) }
.bss : { *(.bss) }

.config1 0x4014 : { 
    KEEP(*(.config1))
}
.config2 0x4016 : { 
    KEEP(*(.config2)) 
}

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

loop:
    goto    loop


#--- config.s

    .section .config1,"ax",@progbits
.byte   0x00, 0x00
    .section .config2,"ax",@progbits
.byte   0x00, 0x00