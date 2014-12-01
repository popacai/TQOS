#include "thread.h"
#include "syscommon.h"
#include "ksyscall.h"
#include "machine.h"
#include "addrspace.h"

void start_child_thread(int argv) {
    //currentThread->space->InitRegisters();
    currentThread->space->RestoreState();		// load page table register
    machine->Run();			// jump to the user progam
}
    
int kfork(int func_ptr) {
    Thread* t;
    AddrSpace* new_space;


    new_space = new AddrSpace(); //a new space
    t = new Thread("child thread");

    new_space->CopyCurrentSpace();
    currentThread->SaveUserState();

    //DEBUG for space
    t->space = new_space;

    machine->WriteRegister(PCReg, func_ptr - 4);
    machine->WriteRegister(NextPCReg, func_ptr);
    t->SaveUserState();
    currentThread->RestoreUserState();
    
    t->Fork(start_child_thread, NULL);
    printf("done_copy\n");
    currentThread->Yield();
}
