include "PICBaseInstrFormats.td"

// TODO: Double check instruction definitions

// BYTE-ORIENTED FILE REGISTER OPERATIONS
def ADDWF : PICBaseByteFileOp<"addwf", 0b000111>;
def ANDWF : PICBaseByteFileOp<"andwf", 0b000101>;
def CLRF : PICBaseByteFileOp_Address<"clrf", 0b000001, 0b1>;
def CLRW : PICBaseByteFileOp_Implicit<"clrw", 0b000001, 0b0>;
def COMF : PICBaseByteFileOp<"comf", 0b001001>;
def DECF : PICBaseByteFileOp<"decf", 0b000011>;
def DECFSZ : PICBaseByteFileOp<"decfsz", 0b001011>, InstConditionalBranch;
def INCF : PICBaseByteFileOp<"incf", 0b001010>; 
def INCFSZ : PICBaseByteFileOp<"incfsz", 0b001111>, InstConditionalBranch; 
def IORWF : PICBaseByteFileOp<"iorwf", 0b000100>; 
def MOVF : PICBaseByteFileOp<"movf", 0b001000>; 
def MOVWF : PICBaseByteFileOp_Address<"movwf", 0b000000, 0b1>; 
def NOP : PICBaseByteFileOp_Implicit<"nop", 0b000000, 0b0>; 
def RLF : PICBaseByteFileOp<"rlf", 0b001101>; 
def RRF : PICBaseByteFileOp<"rrf", 0b001100>; 
def SUBWF : PICBaseByteFileOp<"subwf", 0b000010>; 
def SWAPF : PICBaseByteFileOp<"swapf", 0b001110>; 
def XORWF : PICBaseByteFileOp<"xorwf", 0b000110>;

// BIT-ORIENTED FILE REGISTER OPERATIONS
def BCF : PICBaseBitFileOp<"bcf", 0b0100>; 
def BSF : PICBaseBitFileOp<"bsf", 0b0101>; 
def BTFSC : PICBaseBitFileOp<"btfsc", 0b0110>, InstConditionalBranch;
def BTFSS : PICBaseBitFileOp<"btfss", 0b0111>, InstConditionalBranch;

// LITERAL AND CONTROL OPERATIONS
// NOTE: Above part was pretty much identical to PICBase.
//       The PICBase instructions listed below differ from PICBase.
def ANDLW : PICBaseLiteralAndControlOp<"andlw", 0b1110>; 
def CALL : PICBaseCallOp; 
def CLRWDT : PICBaseLiteralAndControlOp_Implicit<"clrwdt", 0b00000100>; 
def GOTO : PICBaseGotoOp; 
def IORLW : PICBaseLiteralAndControlOp<"iorlw", 0b1101>; 
def MOVLW : PICBaseLiteralAndControlOp<"movlw", 0b1100>; 
def OPTION : PICBaseLiteralAndControlOp_Implicit<"option", 0b00000010>;
def RETLW : PICBaseLiteralAndControlOp<"retlw", 0b1000>, InstReturn; 
def SLEEP : PICBaseLiteralAndControlOp_Implicit<"sleep", 0b00000011>; 
def TRIS : PICBaseTrisOp;
def XORLW : PICBaseLiteralAndControlOp<"xorlw", 0b1111>;
