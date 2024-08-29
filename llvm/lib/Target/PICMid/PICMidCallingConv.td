def PICMid_CSR : CalleeSavedRegs<(sequence "R%u", 10, 15)>;

// Interrupts save all registers except the stack pointers (which are restored
// upon return by any function) and the flags (which are implicitly saved by the
// interrupt).
def PICMid_Interrupt_CSR :
  CalleeSavedRegs<(add W, (sequence "R%u", 1, 15))>;