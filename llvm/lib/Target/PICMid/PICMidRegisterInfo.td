
class PICMidReg<bits<16> num, string name> : Register<!strconcat("llvm_pic_mid_", name)>, DwarfRegNum<[num]> {
    let HWEncoding = num;

    let Namespace = "PICMid";
    let AltNames = [name];
}

// Accumulator Register

def W : PICMidReg<0, "w">;

// Pseudo Registers

defvar ImagRegsAddrOffset = 0x70;
defvar MaxImagRegs = 16;

foreach I = 0...!add(MaxImagRegs, -1) in {
    def R#I: PICMidReg<!add(ImagRegsAddrOffset, I), "rc"#!cast<string>(I)>;
}

// Register Class

class PICMidRegClass<dag RegList>: RegisterClass<"PICMid", [i8], 8, RegList>;

def Wc : PICMidRegClass<(add W)>;
def Imag : PICMidRegClass<(sequence "R%u", 0, !add(MaxImagRegs, -1))>;
