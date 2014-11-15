#include "syscall.h"
#include "syscommon.h"
#include "system.h"

void PushPC() {
    machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
    machine->WriteRegister(PCReg, machine->ReadRegister(PCReg) + 4);
    machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 8); 
}

int read_argument(int index) {
   int ret;
   machine->ReadMem(machine->ReadRegister(StackReg)+16+(index-1)*4,4,&ret);
   return ret;
} 

