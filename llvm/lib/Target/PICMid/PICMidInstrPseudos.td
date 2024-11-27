class PICMidPseudo : Instruction {
  let Namespace = "PICMid";

  dag OutOperandList = (outs);
  dag InOperandList = (ins);

  let hasSideEffects = false;
  let mayLoad = false;
  let mayStore = false;
  let isPseudo = true;
  let isCodeGenOnly = true;
}

class PICMidCallFramePseudo : PICMidPseudo {
    
}

def MOVFromDataMemory : PICMidPseudo {
    let mayLoad = true;
    let isPseudo = true;
    let isCodeGenOnly = true;
}

def MOVToDataMemory : PICMidPseudo {
    let hasSideEffects = true;
    let mayStore = true;
    let isPseudo = true;
    let isCodeGenOnly = true;
}
