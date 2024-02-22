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

def GOTO_L : PICMidUnconditionalBranch {
    let InOperandList = (ins label:$tgt);
    let Pattern = [(br bb:$tgt)];
}

// Addition

class PICMidAddSub : PICMidLogicalInstr {
  dag OutOperandList = (outs Wc:$dst);
  let Constraints = "$dst = $l";
}

class PICMidAddSubOp<Operand op> : PICMidAddSub {
  dag InOperandList = (ins Wc:$l, op:$r);
}

class PICMidAddSubRC<RegisterClass rc> : PICMidAddSub {
  dag InOperandList = (ins Wc:$l, rc:$r);
}

let isAdd = true in {
    def ADDImm : PICMidAddSubOp<imm8>, PseudoInstExpansion<(ADDLW imm8:$r)>;
}

def : Pat<(add Wc:$l, (i8 imm:$imm)), (ADDImm Wc:$l, imm:$imm)>;
