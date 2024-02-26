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

class PICMidInstrResultToF<RegisterClass rc> : PICMidLogicalInstr {
  dag InOperandList = (ins Wc:$l, rc:$r);
  dag OutOperandList = (outs rc:$dst);
  let Constraints = "$dst = $r";
}

class PICMidInstrResultToW<RegisterClass rc> : PICMidLogicalInstr {
  dag InOperandList = (ins Wc:$l, rc:$r);
  dag OutOperandList = (outs rc:$dst);
  let Constraints = "$dst = $l";
}

class PICMidInstrWithLiteral<Operand op> : PICMidLogicalInstr {
  dag InOperandList = (ins Wc:$l, op:$r);
  dag OutOperandList = (outs Wc:$dst);
  let Constraints = "$dst = $l";
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

//def GOTO_L : PICMidUnconditionalBranch {
//    let InOperandList = (ins label:$tgt);
//    let Pattern = [(br bb:$tgt)];
//}

class PICMidReturn<Instruction Opcode> : PICMidLogicalInstr, PseudoInstExpansion<(Opcode)> {
  let isBarrier = true;
  let isReturn = true;
  let isTerminator = true; 
}

def RETURN_L : PICMidReturn<RETURN>;

// Addition / Subtraction

let isAdd = true in {
    def ADDImm : PICMidInstrWithLiteral<imm8>, PseudoInstExpansion<(ADDLW imm8:$r)>;
    def ADDImagToF : PICMidInstrResultToF<Imag>, PseudoInstExpansion<(ADDWF addr7:$r, (imm1 1))>;
    def ADDImagToW : PICMidInstrResultToW<Imag>, PseudoInstExpansion<(ADDWF addr7:$r, (imm1 0))>;
}

def SUBImm : PICMidInstrWithLiteral<imm8>, PseudoInstExpansion<(SUBLW imm8:$r)>;
def SUBImagToF : PICMidInstrResultToF<Imag>, PseudoInstExpansion<(SUBWF addr7:$r, (imm1 1))>;
def SUBImagToW : PICMidInstrResultToW<Imag>, PseudoInstExpansion<(SUBWF addr7:$r, (imm1 0))>;

def : Pat<(add Wc:$l, (i8 imm:$imm)), (ADDImm Wc:$l, imm:$imm)>;
def : Pat<(add Wc:$l, Imag:$r), (ADDImagToF Wc:$l, Imag:$r)>;
def : Pat<(add Wc:$l, Imag:$r), (ADDImagToW Wc:$l, Imag:$r)>;

def : Pat<(sub (i8 imm:$imm), Wc:$l), (SUBImm Wc:$l, imm:$imm)>;
def : Pat<(sub Imag:$r, Wc:$l), (SUBImagToF Wc:$l, Imag:$r)>;
def : Pat<(sub Imag:$r, Wc:$l), (SUBImagToW Wc:$l, Imag:$r)>;

// Byte oriented bitwise operations

