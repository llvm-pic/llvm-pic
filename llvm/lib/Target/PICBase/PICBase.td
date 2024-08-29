include "llvm/Target/Target.td"
include "llvm/Target/GlobalISel/Combine.td"

include "PICBaseFeatures.td"
include "PICBaseDevices.td"
include "PICBaseRegisterInfo.td"
//include "PICBaseRegisterBanks.td"
include "PICBaseInstrInfo.td"
//include "PICBaseCombine.td"

def PICBaseInstrInfo : InstrInfo {
    let guessInstructionProperties = 0;
}

def PICBaseAsmWriter : AsmWriter {
    string AsmWriterClassName = "MCInstPrinter";
    bit isMCAsmWriter = 1;
}

def PICBaseAsmParser : AsmParser {
    let ShouldEmitMatchRegisterAltName = 1;
    let ReportMultipleNearMisses = 0;
    string RegisterPrefix = "";
    string TokenizingCharacters = "#(),[]*!";
}

def PICBaseAsmParserVariantDefault : AsmParserVariant {
    int Variant = 0;
    let Name = "generic";
}

def PICBase : Target {
    let InstructionSet = PICBaseInstrInfo;
    let AssemblyWriters = [PICBaseAsmWriter];
    let AllowRegisterRenaming = 1;

    let AssemblyParsers = [PICBaseAsmParser];
    let AssemblyParserVariants = [PICBaseAsmParserVariantDefault];
}
