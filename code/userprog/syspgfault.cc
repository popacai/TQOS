#include "ksyscall.h"

void kpg_fault() {
    int badAddr;
    
    badAddr = machine->registers[BadVAddrReg];
    printf("bad addr %d\n", badAddr);
    //currentThread->space->PageIn(badAddr);
}
