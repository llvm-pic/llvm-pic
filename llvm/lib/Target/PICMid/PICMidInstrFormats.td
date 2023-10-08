class Inst<string asmstr> : Instruction {
    let Namespace = "PICMid";

    let OutOperandList = (outs);

    let AsmString = asmstr;

    let mayLoad = true;
    let mayStore = true;
    let hasSideEffects = true;

// TODO: Soft Fail?
    let DecoderNamespace = "PICMid";
}

class InstConditionalBranch {
  bit isBranch = true;
  bit isTerminator = true;
}

class InstUnconditionalBranch : InstConditionalBranch {
  bit isBarrier = true;
}

class InstCall {
  bit isCall = true;
}

class InstReturn {
  bit isBarrier = true;
  bit isReturn = true;
  bit isTerminator = true;
}

class PICMidAsmOperand<string name> : AsmOperandClass {
    let Name = name;
    let DiagnosticType = !strconcat("Invalid", name);
}

class ImmediateAsmOperand<string name> : PICMidAsmOperand<name> {
    let RenderMethod = "addImmOperands";
    let DiagnosticType = "immediate";
}

class Addr7Operand<string name> : PICMidAsmOperand<name>;

class PCAbsoluteOperand<string name> : PICMidAsmOperand<name>;

def imm1 : Operand<i32> {
    let ParserMatchClass = ImmediateAsmOperand<"Imm1">;
    let DecoderMethod = "decodeUImmOperand<1>";
    let OperandType = "OPERAND_IMM1";
    let OperandNamespace = "PICMidOp";
    let Type = i1;
}

def imm3 : Operand<i32> {
    let ParserMatchClass = ImmediateAsmOperand<"Imm3">;
    let DecoderMethod = "decodeUImmOperand<3>";
    let OperandType = "OPERAND_IMM3";
    let OperandNamespace = "PICMidOp";
    let Type = i8;
}

def imm8 : Operand<i32> {
    let ParserMatchClass = ImmediateAsmOperand<"Imm8">;
    // TODO: Investigate `let EncoderMethod = "encodeImm<PICMid::Imm8, 1>";`
    let EncoderMethod = "encodeImm<PICMid::Imm8, 1>";
    let DecoderMethod = "decodeUImmOperand<8>";
    let OperandType = "OPERAND_IMM8";
    let OperandNamespace = "PICMidOp";
    let Type = i8;
}

def pcabs11 : Operand<i32> {
    let ParserMatchClass = PCAbsoluteOperand<"PCAbs11">;
    let EncoderMethod = "encodeImm<PICMid::PCAbs11, 1>";
    let DecoderMethod = "decodeSImmOperand<11>";
    let PrintMethod = "printBranchOperand";
    let OperandType = "OPERAND_PCABS";
}

def addr7 : Operand<i32> {
    let ParserMatchClass = Addr7Operand<"Addr7">;
    let EncoderMethod = "encodeImm<PICMid::Addr7, 1>";
    let DecoderMethod = "decodeAddr7Operand";
    let OperandType = "OPERAND_ADDR7";
    let OperandNamespace = "PICMidOp";
    let Type = i8;
}

class AddressingMode {
    string OperandsStr = ?;
    dag InOperandList = ?;
}

def Implicit : AddressingMode {
    string OperandsStr = "";
    dag InOperandList = (ins);
}

def AddressWithSaveDirection : AddressingMode {
    let OperandsStr = "$f, $d";
    let InOperandList = (ins addr7:$f, imm1:$d);
}

def Address : AddressingMode {
    let OperandsStr = "$f";
    let InOperandList = (ins addr7:$f);
}

def AddressWithDesiredBit : AddressingMode {
    let OperandsStr = "$f, $b";
    let InOperandList = (ins addr7:$f, imm3:$b);
}

def Immediate : AddressingMode {
    let OperandsStr = "$k";
    let InOperandList = (ins imm8:$k);
}

def PCAbsolute : AddressingMode {
    let OperandsStr = "$k";
    let InOperandList = (ins pcabs11:$k);
}

class InstWithAddressingMode<string asmstr, AddressingMode mode = Implicit> : Inst<asmstr> {
    let InOperandList = mode.InOperandList;
    string OperandsStr;
    let OperandsStr = mode.OperandsStr; // FIXME: Necessary?
}

class PICMidInst<string opcodestr, AddressingMode mode = Implicit> : InstWithAddressingMode<opcodestr, mode> {
    let Size = 1;
    bits<14> Inst;

    let AsmString = opcodestr # " " # mode.OperandsStr;
}

// Corresponds to "BYTE-ORIENTED FILE REGISTER OPERATIONS"
// TODO: May want to check if this work with DAG and stuff
class PICMidByteFileOp<string opcodestr, bits<6> opcode, bit dOpt = 0, bits<7> fOpt = 0, AddressingMode mode = AddressWithSaveDirection> : PICMidInst<opcodestr, mode> {
    bit d = dOpt;
    bits<7> f = fOpt;
    let Inst{13-8} = opcode;
    let Inst{7} = d;
    let Inst{6-0} = f;
}

// Corresponds to "BIT-ORIENTED FILE REGISTER OPERATIONS"
// TODO: May want to check if this work with DAG and stuff
class PICMidBitFileOp<string opcodestr, bits<4> opcode, AddressingMode mode = AddressWithDesiredBit> : PICMidInst<opcodestr, mode> {
    bits<3> b;
    bits<7> f;
    let Inst{13-10} = opcode;
    let Inst{9-7} = b;
    let Inst{6-0} = f;
}

// Corresponds to "LITERAL AND CONTROL OPERATIONS"
// TODO: May want to check if this work with DAG and stuff
class PICMidLiteralAndControlOp<string opcodestr, bits<6> opcode, bits<8> kOpt = 0, AddressingMode mode = Immediate> : PICMidInst<opcodestr, mode> {
    bits<8> k = kOpt;
    let Inst{13-8} = opcode;
    let Inst{7-0} = k;
}

class PICMidCallOrGotoOp<string opcodestr, bits<3> opcode> : PICMidInst<opcodestr, PCAbsolute> {
    bits<11> k;
    let Inst{13-11} = opcode;
    let Inst{10-0} = k;
}
