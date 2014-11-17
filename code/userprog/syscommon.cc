#include "syscall.h"
#include "system.h"

void PushPC() {
    machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
    machine->WriteRegister(PCReg, machine->ReadRegister(PCReg) + 4);
    machine->WriteRegister(NextPCReg, machine->ReadRegister(PCReg) + 8); 
}

int ustrlen(int src) {
    int n, value;
    n = 0;

    do {
        if (!(machine->ReadMem(src + n, 1, &value)))
          return -1;
        n++;
    } while (value != 0);

    return n - 1;
}

int u2kmemcpy(unsigned char* dst, int src, int n) {
    int value;
    int i;
    for (i = 0; i < n; i++) {
        if (machine->ReadMem(src + i, 1, &value))
            *(dst + i) = value;
        else
            return i;
    }
    return i;
}

int u2kmemread(int src) {
    int value;
    if (!(machine->ReadMem(src, 4, &value))) {
        ASSERT(false);
    }
    return value;
}

int u2kmatrixcpy(unsigned char** dst, int src, int n) {
   int i, len;
   for (i = 0; i < n; i++) {
        int addr;
        machine->ReadMem(src + i*4,4,&addr);
        len = ustrlen(addr);
        dst[i] = new unsigned char[len];
        u2kmemcpy(dst[i],addr,len);
   }
   return i;
}

