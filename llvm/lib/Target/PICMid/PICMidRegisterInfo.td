class PICMidSubRegIndex<int size, int offset = 0>: SubRegIndex<size, offset> {
  let Namespace = "PICMid";
}

def subcarry : PICMidSubRegIndex<0>;
def subdigitalcarry : PICMidSubRegIndex<1>;
def subzero : PICMidSubRegIndex<2>;
def subpowerdown : PICMidSubRegIndex<3>;
def subtimeout : PICMidSubRegIndex<4>;

class PICMidReg<bits<16> num, string name> : Register<!strconcat("llvm_pic_mid_", name)>, DwarfRegNum<[num]> {
    let HWEncoding = num;
    let Namespace = "PICMid";
    let AltNames = [name];
}

// Accumulator Register

// Dwarf Number, register string
def W : PICMidReg<0, "w">;
def C : PICMidReg<1, "C">;
def DC : PICMidReg<2, "DC">;
def Z : PICMidReg<3, "Z">;
def PD : PICMidReg<4, "PD">;
def TO : PICMidReg<5, "TO">;
// TODO add register selection registers
def STATUS : PICMidReg<3, "STATUS"> {
    let SubRegs = [C, DC, Z, PD, TO];
    let SubRegIndices = [subcarry, subdigitalcarry, subzero, subpowerdown, subtimeout];
}

// Pseudo Registers

defvar ImagRegsAddrOffset = 0x70;
defvar MaxImagRegs = 16;

foreach I = 0...!add(MaxImagRegs, -1) in {
    def R#I: PICMidReg<!add(ImagRegsAddrOffset, I), "r"#!cast<string>(I)>;
}

// Register Class

class PICMidRegClass<list<ValueType> regTypes, int alignment, dag RegList>
    : RegisterClass<"PICMid", regTypes, alignment, RegList>;
class PICMidReg1Class<dag RegList> : PICMidRegClass<[i1], 8, RegList> {
  // The register is 1-bit wide, but takes 8 bits to spill.
  let RegInfos = RegInfoByHwMode<[DefaultMode], [RegInfo<1, 8, 8>]>;
}
class PICMidReg8Class<dag RegList> : PICMidRegClass<[i8], 8, RegList>;

def Wc : PICMidReg8Class<(add W)>;
def Cc : PICMidReg1Class<(add C)>;
def DCc : PICMidReg1Class<(add DC)>;
def Zc : PICMidReg1Class<(add Z)>;
def PDc : PICMidReg1Class<(add PD)>;
def TOc : PICMidReg1Class<(add TO)>;
def STATUSc : PICMidReg8Class<(add STATUS)>;

def Imag : PICMidReg8Class<(sequence "R%u", 0, !add(MaxImagRegs, -1))>;
