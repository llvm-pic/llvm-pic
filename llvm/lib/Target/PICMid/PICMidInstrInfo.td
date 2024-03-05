include "PICMidInstrFormats.td"

// TODO: Double check instruction definitions

// BYTE-ORIENTED FILE REGISTER OPERATIONS
def ADDWF : PICMidByteFileOp<"addwf", 0b000111>;
def ANDWF : PICMidByteFileOp<"andwf", 0b000101>;
def CLRF : PICMidByteFileOp_Address<"clrf", 0b000001, 0b1>;
def CLRW : PICMidByteFileOp_Implicit<"clrw", 0b000001, 0b0>;
def COMF : PICMidByteFileOp<"comf", 0b001001>;
def DECF : PICMidByteFileOp<"decf", 0b000011>;
def DECFSZ : PICMidByteFileOp<"decfsz", 0b001011>, InstConditionalBranch;
def INCF : PICMidByteFileOp<"incf", 0b001010>; 
def INCFSZ : PICMidByteFileOp<"incfsz", 0b001111>, InstConditionalBranch; 
def IORWF : PICMidByteFileOp<"iorwf", 0b000100>; 
def MOVF : PICMidByteFileOp<"movf", 0b001000>; 
def MOVWF : PICMidByteFileOp_Address<"movwf", 0b000000, 0b1>; 
def NOP : PICMidByteFileOp_Implicit<"nop", 0b000000, 0b0>; 
def RLF : PICMidByteFileOp<"rlf", 0b001101>; 
def RRF : PICMidByteFileOp<"rrf", 0b001100>; 
def SUBWF : PICMidByteFileOp<"subwf", 0b000010>; 
def SWAPF : PICMidByteFileOp<"swapf", 0b001110>; 
def XORWF : PICMidByteFileOp<"xorwf", 0b000110>;

// BIT-ORIENTED FILE REGISTER OPERATIONS
def BCF : PICMidBitFileOp<"bcf", 0b0100>; 
def BSF : PICMidBitFileOp<"bsf", 0b0101>; 
def BTFSC : PICMidBitFileOp<"btfsc", 0b0110>, InstConditionalBranch; 
def BTFSS : PICMidBitFileOp<"btfss", 0b0111>, InstConditionalBranch;

// LITERAL AND CONTROL OPERATIONS
def ADDLW : PICMidLiteralAndControlOp<"addlw", 0b111110>;
def ANDLW : PICMidLiteralAndControlOp<"andlw", 0b111001>; 
def CALL : PICMidCallOrGotoOp<"call", 0b100>, InstCall; 
def CLRWDT : PICMidLiteralAndControlOp_Implicit<"clrwdt", 0b01100100>; 
def GOTO : PICMidCallOrGotoOp<"goto", 0b101>;
def IORLW : PICMidLiteralAndControlOp<"iorlw", 0b111000>; 
def MOVLW : PICMidLiteralAndControlOp<"movlw", 0b110000>; 
def RETFIE : PICMidLiteralAndControlOp_Implicit<"retfie", 0b00000001>, InstReturn; 
def RETLW : PICMidLiteralAndControlOp<"retlw", 0b110100>, InstReturn; 
def RETURN : PICMidLiteralAndControlOp_Implicit<"return", 0b00001000>, InstReturn; 
def SLEEP : PICMidLiteralAndControlOp_Implicit<"sleep", 0b01100011>; 
def SUBLW : PICMidLiteralAndControlOp<"sublw", 0b111100>; 
def XORLW : PICMidLiteralAndControlOp<"xorlw", 0b111010>;

// Operand type for basic block and function labels.
def label : Operand<OtherVT>;

include "PICMidInstrGISel.td"
include "PICMidInstrLogical.td"
include "PICMidInstrPseudos.td"
