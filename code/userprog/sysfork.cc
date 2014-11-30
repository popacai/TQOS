#include "thread.h"
#include "syscommon.h"
#include "ksyscall.h"
#include "machine.h"
#include "translate.h"
#include "addrspace.h"

/*void StartChildProcess(int argv) {
    int numPages;
    numPages = currentThread->space->getNumPages();
    PushPC();
    machine->WriteRegister(StackReg, numPages*PageSize - 256);
    machine->WriteRegister(2,0);
    machine->Run();
}*/
    
void kfork(int arg_t) {
    Thread* t;
    //int foo_addr;
    void (*foo)(int);  
    //machine->ReadMem(arg_t,4,&foo_addr);
    //machine->Translate(arg_t, &foo_addr, 4, 0);
    AddrSpace *space = currentThread->space;
    
    printf("%d\n",arg_t);
    //printf("%d\n",foo_addr);
    foo = (void (*)(int))arg_t;
    printf("%d\n",(int)foo);
    t = new Thread("child thread");
    t->space = space;
    t->Fork(foo,NULL);
    currentThread->Yield();
}
