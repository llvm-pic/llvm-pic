class Inst<string asmstr> : Instruction {
    let Namespace = "PICBase";

    let OutOperandList = (outs);

    let AsmString = asmstr;

    let mayLoad = true;
    let mayStore = true;
    let hasSideEffects = true;

    let DecoderNamespace = "PICBase";
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

class PICBaseAsmOperand<string name> : AsmOperandClass {
    let Name = name;
    let DiagnosticType = !strconcat("Invalid", name);
}

class ImmediateAsmOperand<string name> : PICBaseAsmOperand<name> {
    let RenderMethod = "addImmOperands";
    let DiagnosticType = "immediate";
}

class AddrOperand<string name> : PICBaseAsmOperand<name>;

class PCAbsoluteOperand<string name> : PICBaseAsmOperand<name>;

def imm1 : Operand<i32> {
    let ParserMatchClass = ImmediateAsmOperand<"Imm1">;
    let DecoderMethod = "decodeUImmOperand<1>";
    let OperandType = "OPERAND_IMM1";
    let OperandNamespace = "PICBaseOp";
    let Type = i1;
}

def imm3 : Operand<i32> {
    let ParserMatchClass = ImmediateAsmOperand<"Imm3">;
    let DecoderMethod = "decodeUImmOperand<3>";
    let OperandType = "OPERAND_IMM3";
    let OperandNamespace = "PICBaseOp";
    let Type = i8;
}

def imm8 : Operand<i32> {
    let ParserMatchClass = ImmediateAsmOperand<"Imm8">;
    let EncoderMethod = "encodeImm<PICBase::Imm8, 0>";
    let DecoderMethod = "decodeUImmOperand<8>";
    let OperandType = "OPERAND_IMM8";
    let OperandNamespace = "PICBaseOp";
    let Type = i8;
}

def pcabs8 : Operand<i32> {
    let ParserMatchClass = PCAbsoluteOperand<"PCAbs8">;
    let EncoderMethod = "encodeImm<PICBase::PCAbs8, 0>";
    let DecoderMethod = "decodeSImmOperand<8>";
    let PrintMethod = "printBranchOperand";
    let OperandType = "OPERAND_PCABS8";
    let OperandNamespace = "PICBaseOp";
}
def pcabs9 : Operand<i32> {
    let ParserMatchClass = PCAbsoluteOperand<"PCAbs9">;
    let EncoderMethod = "encodeImm<PICBase::PCAbs9, 0>";
    let DecoderMethod = "decodeSImmOperand<9>";
    let PrintMethod = "printBranchOperand";
    let OperandType = "OPERAND_PCABS9";
    let OperandNamespace = "PICBaseOp";
}

// FIXME: Can only be 6 or 7. Must be checked.
def addr3 : Operand<i32> {
    let ParserMatchClass = AddrOperand<"Addr3">;
    let EncoderMethod = "encodeImm<PICBase::Addr3, 0>";
    let DecoderMethod = "decodeAddr3Operand";
    let OperandType = "OPERAND_ADDR3";
    let OperandNamespace = "PICBaseOp";
    let Type = i8;
}

def addr5 : Operand<i32> {
    let ParserMatchClass = AddrOperand<"Addr5">;
    let EncoderMethod = "encodeImm<PICBase::Addr5, 0>";
    let DecoderMethod = "decodeAddr5Operand";
    let OperandType = "OPERAND_ADDR5";
    let OperandNamespace = "PICBaseOp";
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
    let InOperandList = (ins addr5:$f, imm1:$d);
}

def Address : AddressingMode {
    let OperandsStr = "$f";
    let InOperandList = (ins addr5:$f);
}

def TrisAddress : AddressingMode {
    let OperandsStr = "$f";
    let InOperandList = (ins addr3:$f);
}

def AddressWithDesiredBit : AddressingMode {
    let OperandsStr = "$f, $b";
    let InOperandList = (ins addr5:$f, imm3:$b);
}

def Immediate : AddressingMode {
    let OperandsStr = "$k";
    let InOperandList = (ins imm8:$k);
}

def PCAbsolute8 : AddressingMode {
    let OperandsStr = "$k";
    let InOperandList = (ins pcabs8:$k);
}

def PCAbsolute9 : AddressingMode {
    let OperandsStr = "$k";
    let InOperandList = (ins pcabs9:$k);
}

class InstWithAddressingMode<string asmstr, AddressingMode mode = Implicit> : Inst<asmstr> {
    let InOperandList = mode.InOperandList;
    string OperandsStr;
    let OperandsStr = mode.OperandsStr;
}

class PICBaseInst<string opcodestr, AddressingMode mode = Implicit> : InstWithAddressingMode<opcodestr, mode> {
    let Size = 2;
    bits<12> Inst;

    let AsmString = opcodestr # " " # mode.OperandsStr;
}

// Corresponds to "BYTE-ORIENTED FILE REGISTER OPERATIONS"
// TODO: May want to check if this work with DAG and stuff
class PICBaseByteFileOp<string opcodestr, bits<6> opcode> : PICBaseInst<opcodestr, AddressWithSaveDirection> {
    bit d;
    bits<5> f;
    let Inst{11-6} = opcode;
    let Inst{5} = d;
    let Inst{4-0} = f;
}
class PICBaseByteFileOp_Address<string opcodestr, bits<6> opcode, bit d> : PICBaseInst<opcodestr, Address> {
    bits<5> f;
    let Inst{11-6} = opcode;
    let Inst{5} = d;
    let Inst{4-0} = f;
}
class PICBaseByteFileOp_Implicit<string opcodestr, bits<6> opcode, bit d> : PICBaseInst<opcodestr, Implicit> {
    let Inst{11-6} = opcode;
    let Inst{5} = d;
    let Inst{4-0} = 0;
}

// Corresponds to "BIT-ORIENTED FILE REGISTER OPERATIONS"
// TODO: May want to check if this work with DAG and stuff
class PICBaseBitFileOp<string opcodestr, bits<4> opcode, AddressingMode mode = AddressWithDesiredBit> : PICBaseInst<opcodestr, mode> {
    bits<3> b;
    bits<5> f;
    let Inst{11-8} = opcode;
    let Inst{7-5} = b;
    let Inst{4-0} = f;
}

// Corresponds to "LITERAL AND CONTROL OPERATIONS"
// TODO: May want to check if this work with DAG and stuff
class PICBaseLiteralAndControlOp<string opcodestr, bits<4> opcode> : PICBaseInst<opcodestr, Immediate> {
    bits<8> k;
    let Inst{11-8} = opcode;
    let Inst{7-0} = k;
}

class PICBaseLiteralAndControlOp_Implicit<string opcodestr, bits<8> kOpt> : PICBaseInst<opcodestr, Implicit> {
    let Inst{11-8} = 0;
    let Inst{7-0} = kOpt;
}

class PICBaseGotoOp : PICBaseInst<"goto", PCAbsolute9>, InstUnconditionalBranch {
    bits<9> k;
    let Inst{11-9} = 0b101;
    let Inst{8-0} = k;
}

class PICBaseCallOp : PICBaseInst<"call", PCAbsolute8>, InstCall {
    bits<8> k;
    let Inst{11-8} = 0b1001;
    let Inst{7-0} = k;
}

// Special instruction
class PICBaseTrisOp : PICBaseInst<"tris", TrisAddress> {
    bits<3> f;
    let Inst{11-3} = 0;
    let Inst{2-0} = f;
}