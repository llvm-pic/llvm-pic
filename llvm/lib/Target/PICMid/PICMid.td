include "llvm/Target/Target.td"
include "llvm/Target/GlobalISel/Combine.td"

include "PICMidFeatures.td"
include "PICMidDevices.td"
include "PICMidRegisterInfo.td"
include "PICMidRegisterBanks.td"
include "PICMidInstrInfo.td"


def PICMidInstrInfo : InstrInfo {
    let guessInstructionProperties = 0;
}

include "PICMidCallingConv.td"

def PICMidAsmWriter : AsmWriter {
    string AsmWriterClassName = "MCInstPrinter";
    bit isMCAsmWriter = 1;
}

def PICMidAsmParser : AsmParser {
    let ShouldEmitMatchRegisterAltName = 1;
    let ReportMultipleNearMisses = 0;
    string RegisterPrefix = "";
    string TokenizingCharacters = "#(),[]*!";
}

def PICMidAsmParserVariantDefault : AsmParserVariant {
    int Variant = 0;
    let Name = "generic";
}

def PICMid : Target {
    let InstructionSet = PICMidInstrInfo;
    let AssemblyWriters = [PICMidAsmWriter];
    let AllowRegisterRenaming = 1;

    let AssemblyParsers = [PICMidAsmParser];
    let AssemblyParserVariants = [PICMidAsmParserVariantDefault];
}
