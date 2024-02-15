
class PICBaseReg<bits<16> num, string name> : Register<!strconcat("llvm_pic_mid_", name)>, DwarfRegNum<[num]> {
    let HWEncoding = num;

    let Namespace = "PICBase";
    let AltNames = [name];
}

// Accumulator Register

def W : PICBaseReg<0, "w">;

// Pseudo Registers

defvar ImagRegsAddrOffset = 0x70;
defvar MaxImagRegs = 16;

foreach I = 0...!add(MaxImagRegs, -1) in {
    def R#I: PICBaseReg<!add(ImagRegsAddrOffset, I), "rc"#!cast<string>(I)>;
}

// Register Class

class PICBaseRegClass<dag RegList>: RegisterClass<"PICBase", [i8], 8, RegList>;

def Wc : PICBaseRegClass<(add W)>;
def Imag : PICBaseRegClass<(sequence "R%u", 0, !add(MaxImagRegs, -1))>;
