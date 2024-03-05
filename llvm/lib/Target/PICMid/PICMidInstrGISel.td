

class PICMidGenericInstruction : GenericInstruction {
  let Namespace = "PICMid";
  let hasSideEffects = false;
}

// Shift an 8-bit value left one bit, placing $carry_in in the LSB and placing
// the previous MSB bit into $carry_out. This "even" version can be chained to
// perform multi-byte shifts.
def G_SHLE : PICMidGenericInstruction {
  let OutOperandList = (outs type0:$dst, type1:$carry_out);
  let InOperandList = (ins type0:$src, type1:$carry_in);
}

// Logically shift an 8-bit value right one bit, placing $carry_in in the MSB
// and placing the previous LSB bit into $carry_out. This "even" version can be
// chained to perform multi-byte shifts.
def G_LSHRE : PICMidGenericInstruction {
  let OutOperandList = (outs type0:$dst, type1:$carry_out);
  let InOperandList = (ins type0:$src, type1:$carry_in);
}