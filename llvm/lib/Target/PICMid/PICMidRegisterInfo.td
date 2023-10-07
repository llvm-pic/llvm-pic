
class PICMidReg<bits<16> num, string name> : Register<!strconcat("llvm_pic_mid_", name)>, DwarfRegNum<[num]> {
    let HWEncoding = num;

    let Namespace = "PICMid";
    let AltNames = [name];
}

def W : PICMidReg<0, "w">;

class PICMidRegClass<dag RegList>: RegisterClass<"PICMid", [i8], 8, RegList>;

def Wc : PICMidRegClass<(add W)>;
