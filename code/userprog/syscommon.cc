#include "syscall.h"

void PushPC() {
    machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
    machine->WriteRegister(PCReg, machine->ReadRegister(PCReg) + 4);
    machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 8); 
}

