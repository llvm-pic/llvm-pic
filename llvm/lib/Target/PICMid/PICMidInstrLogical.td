//===- PICMidInstrLogical.td - Describe PICMid Logical Instrs -*- tablegen -*-===//
//
// Part of LLVM-PIC, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===-------------------------------------------------------------------------===//
//
// This file describes the logical instruction set for mid-range PIC core
// devices.
//
// Inspired by LLVM-MOS instructions are normalized to reduce copy and pasting
// of instruction patterns.
//
// FIXME: Although for our use case, normalization does not offer any benefits
//        because PIC's instruction set is simple. Perhaps its better to get
//        rid of this file.
// FIXME: We may also look into delegating some of the work done during
//        instruction selection to the Legalizer.
//
//===------------------------------------------------------------------------===//


// TODO add the affected register classes

class PICMidLogicalInstr : Instruction {
  let Namespace = "PICMid";

  dag OutOperandList = (outs);
  dag InOperandList = (ins);

  let hasSideEffects = false;
  let mayLoad = false;
  let mayStore = false;
  let isPseudo = true;
  let isCodeGenOnly = true;
  let isReMaterializable = true;
}

class PICMidBinaryInstrResultToF<RegisterClass rc> : PICMidLogicalInstr {
  dag InOperandList = (ins Wc:$w, rc:$f);
  dag OutOperandList = (outs rc:$dst);
  let Constraints = "$dst = $f";
}

class PICMidBinaryInstrResultToW<RegisterClass rc> : PICMidLogicalInstr {
  dag InOperandList = (ins Wc:$w, rc:$f);
  dag OutOperandList = (outs Wc:$dst);
  let Constraints = "$dst = $w";
}

class PICMidBinaryInstrWithLiteral<Operand op> : PICMidLogicalInstr {
  dag InOperandList = (ins Wc:$w, op:$k);
  dag OutOperandList = (outs Wc:$dst);
  let Constraints = "$dst = $w";
}

class PICMidUnaryInstrResultToF<RegisterClass rc> : PICMidLogicalInstr {
  dag InOperandList = (ins rc:$f);
  dag OutOperandList = (outs rc:$dst);
  let Constraints = "$dst = $f";
}

class PICMidUnaryInstrResultToW<RegisterClass rc> : PICMidLogicalInstr {
  dag InOperandList = (ins rc:$f);
  dag OutOperandList = (outs Wc:$dst);
}

// Control Flow

class PICMidBranch : PICMidLogicalInstr {
  let isBranch = true;
  let isTerminator = true;
}

// Covers all branch instructions. Branches to $tgt if $flag = $val.
def BR : PICMidBranch {
  dag InOperandList = (ins label:$tgt, i1imm:$val);
}

class PICMidUnconditionalBranch : PICMidBranch {
  let isBarrier = true;
}

// consume 
class PICMidConditionalBranch : PICMidBranch {
  let isBarrier = true;
}

def G_GOTO_L : PICMidUnconditionalBranch {
   let InOperandList = (ins label:$tgt);
   let Pattern = [(br bb:$tgt)];
}

def G_CALL : PICMidLogicalInstr, PseudoInstExpansion<(CALL pcabs11:$k)> {
    dag InOperandList = (ins label:$k);
    let isCall = true;
}

// Return

class PICMidReturn<dag Opcode> : PICMidLogicalInstr, PseudoInstExpansion<Opcode> {
  let isBarrier = true;
  let isReturn = true;
  let isTerminator = true; 
}

def G_RETURN : PICMidReturn<(RETURN)>;
def G_RETFIE : PICMidReturn<(RETFIE)>;
def G_RETLW : PICMidReturn<(RETLW imm8:$k)> {
    dag InOperandList = (ins imm8:$k);
}

// Addition / Subtraction

let isAdd = true in {
    def G_ADDLW : PICMidBinaryInstrWithLiteral<imm8>, PseudoInstExpansion<(ADDLW imm8:$k)>;
    def G_ADDWF_F : PICMidBinaryInstrResultToF<Imag>, PseudoInstExpansion<(ADDWF addr7:$f, (imm1 1))>;
    def G_ADDWF_W : PICMidBinaryInstrResultToW<Imag>, PseudoInstExpansion<(ADDWF addr7:$f, (imm1 0))>;

    def G_INCF_F : PICMidUnaryInstrResultToF<Imag>, PseudoInstExpansion<(INCF addr7:$f, (imm1 1))>;
    def G_INCF_W : PICMidUnaryInstrResultToW<Imag>, PseudoInstExpansion<(INCF addr7:$f, (imm1 0))>;
}

def G_SUBLW : PICMidBinaryInstrWithLiteral<imm8>, PseudoInstExpansion<(SUBLW imm8:$k)>;
def G_SUBWF_F : PICMidBinaryInstrResultToF<Imag>, PseudoInstExpansion<(SUBWF addr7:$f, (imm1 1))>;
def G_SUBWF_W : PICMidBinaryInstrResultToW<Imag>, PseudoInstExpansion<(SUBWF addr7:$f, (imm1 0))>;

def G_DECF_F : PICMidUnaryInstrResultToF<Imag>, PseudoInstExpansion<(DECF addr7:$f, (imm1 1))>;
def G_DECF_W : PICMidUnaryInstrResultToW<Imag>, PseudoInstExpansion<(DECF addr7:$f, (imm1 0))>;

def : Pat<(add Wc:$w, (i8 imm:$imm)), (G_ADDLW Wc:$w, imm:$imm)>;
// def : Pat<(add Wc:$k, (i8 imm:$imm)), (ADDLW Wc:$l, imm:$imm)>;
def : Pat<(add Wc:$w, Imag:$r), (G_ADDWF_F Wc:$w, Imag:$r)>;
def : Pat<(add Wc:$w, Imag:$r), (G_ADDWF_W Wc:$w, Imag:$r)>;

def : Pat<(sub (i8 imm:$imm), Wc:$w), (G_SUBLW Wc:$w, imm:$imm)>;
def : Pat<(sub Imag:$r, Wc:$w), (G_SUBWF_F Wc:$w, Imag:$r)>;
def : Pat<(sub Imag:$r, Wc:$w), (G_SUBWF_W Wc:$w, Imag:$r)>;

def : Pat<(add Imag:$r, (i8 1)), (G_INCF_F Imag:$r)>;
def : Pat<(add Imag:$r, (i8 1)), (G_INCF_W Imag:$r)>;

def : Pat<(sub Imag:$r, (i8 1)), (G_DECF_F Imag:$r)>;
def : Pat<(sub Imag:$r, (i8 1)), (G_DECF_W Imag:$r)>;

// Byte oriented bitwise operations

class PICMidRotateResultToF<RegisterClass rc, RegisterClass rcCarry> : PICMidUnaryInstrResultToF<rc> {
    dag InOperandList = (ins rc:$f, rcCarry:$cIn);
    dag OutOperandList = (outs rc:$dst, rcCarry:$cOut);
    let Constraints = "$f = $dst";
}
class PICMidRotateResultToW<RegisterClass rc, RegisterClass rcCarry> : PICMidUnaryInstrResultToF<rc> {
    dag InOperandList = (ins rc:$f, rcCarry:$cIn);
    dag OutOperandList = (outs Wc:$dst, rcCarry:$cOut);
}

def G_IORLW : PICMidBinaryInstrWithLiteral<imm8>, PseudoInstExpansion<(IORLW imm8:$k)>;
def G_IORWF_F : PICMidBinaryInstrResultToF<Imag>, PseudoInstExpansion<(IORWF addr7:$f, (imm1 1))>;
def G_IORWF_W : PICMidBinaryInstrResultToW<Imag>, PseudoInstExpansion<(IORWF addr7:$f, (imm1 0))>;

def G_ANDLW : PICMidBinaryInstrWithLiteral<imm8>, PseudoInstExpansion<(ANDLW imm8:$k)>;
def G_ANDWF_F : PICMidBinaryInstrResultToF<Imag>, PseudoInstExpansion<(ANDWF addr7:$f, (imm1 1))>;
def G_ANDWF_W : PICMidBinaryInstrResultToW<Imag>, PseudoInstExpansion<(ANDWF addr7:$f, (imm1 0))>;

def G_XORLW : PICMidBinaryInstrWithLiteral<imm8>, PseudoInstExpansion<(XORLW imm8:$k)>;
def G_XORWF_F : PICMidBinaryInstrResultToF<Imag>, PseudoInstExpansion<(XORWF addr7:$f, (imm1 1))>;
def G_XORWF_W : PICMidBinaryInstrResultToW<Imag>, PseudoInstExpansion<(XORWF addr7:$f, (imm1 0))>;

def G_COMF_F : PICMidUnaryInstrResultToF<Imag>, PseudoInstExpansion<(COMF addr7:$f, (imm1 1))>;
def G_COMF_W : PICMidUnaryInstrResultToW<Imag>, PseudoInstExpansion<(COMF addr7:$f, (imm1 0))>;

// TODO: Implement instruction selection for rotating through carry
def G_RLF_F : PICMidRotateResultToF<Imag, Cc>, PseudoInstExpansion<(RLF addr7:$f, (imm1 1))>;
def G_RLF_W : PICMidRotateResultToW<Imag, Cc>, PseudoInstExpansion<(RLF addr7:$f, (imm1 0))>;

def G_RRF_F : PICMidRotateResultToF<Imag, Cc>, PseudoInstExpansion<(RRF addr7:$f, (imm1 1))>;
def G_RRF_W : PICMidRotateResultToW<Imag, Cc>, PseudoInstExpansion<(RRF addr7:$f, (imm1 0))>;

def : Pat<(or Wc:$w, (i8 imm:$imm)), (G_IORLW Wc:$w, imm:$imm)>;
def : Pat<(or Wc:$w, Imag:$r), (G_IORWF_F Wc:$w, Imag:$r)>;
def : Pat<(or Wc:$w, Imag:$r), (G_IORWF_W Wc:$w, Imag:$r)>;

def : Pat<(and Wc:$w, (i8 imm:$imm)), (G_ANDLW Wc:$w, imm:$imm)>;
def : Pat<(and Wc:$w, Imag:$r), (G_ANDWF_F Wc:$w, Imag:$r)>;
def : Pat<(and Wc:$w, Imag:$r), (G_ANDWF_W Wc:$w, Imag:$r)>;

def : Pat<(xor Wc:$w, (i8 imm:$imm)), (G_XORLW Wc:$w, imm:$imm)>;
def : Pat<(xor Wc:$w, Imag:$r), (G_XORWF_F Wc:$w, Imag:$r)>;
def : Pat<(xor Wc:$w, Imag:$r), (G_XORWF_W Wc:$w, Imag:$r)>;

def : Pat<(xor Imag:$r, (i8 -1)), (G_COMF_F Imag:$r)>;
def : Pat<(xor Imag:$r, (i8 -1)), (G_COMF_W Imag:$r)>;

//def : Pat<(G_LSHRE Imag:$r, Cc:$c), (G_RRF_F Imag:$r, Cc:$c)>;

// Miscellaneous instruction

class PICMidClearF<RegisterClass rc> : PICMidLogicalInstr {
    dag InOperandList = (ins rc:$f);
    let hasSideEffects = true;
    let mayStore = true;
}

def G_CLRF : PICMidClearF<Imag>, PseudoInstExpansion<(CLRF addr7:$f)>; // TODO: Figure out how to do this
def G_MOVLW : PICMidLogicalInstr, PseudoInstExpansion<(MOVLW imm8:$k)> {
    dag InOperandList = (ins imm8:$k);
    dag OutOperandList = (outs Wc:$dst);
}
def G_MOVF_W : PICMidUnaryInstrResultToW<Imag>, PseudoInstExpansion<(MOVF addr7:$f, (imm1 0))>;
def G_MOVWF : PICMidLogicalInstr, PseudoInstExpansion<(MOVWF addr7:$f)> {
    dag InOperandList = (ins Wc:$w);
    dag OutOperandList = (outs Imag:$f);
}

// Integer Comparison

class PICMidIntegerComparison<RegisterClass rc> : PICMidLogicalInstr {
  dag InOperandList = (ins Wc:$w, rc:$f);
  dag OutOperandList = (outs Wc:$w, Zc:$z);
}

// TODO
def G_ICMP_LW : PICMidBinaryInstrWithLiteral<imm8>, PseudoInstExpansion<(SUBLW imm8:$k)>;
def G_ICMP_W : PICMidBinaryInstrResultToW<Imag>, PseudoInstExpansion<(SUBWF addr7:$f, (imm1 0))>;


//def : Pat<(store 0, Imag:$r), (G_CLRF Imag:$r)>;
def : Pat<(i8 imm:$k), (G_MOVLW imm:$k)>;
