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

    //DEBUG for space
    t->space = new_space;
    t->SaveUserState();
    
    t->Fork(start_child_thread, NULL);
    printf("done_copy\n");
    currentThread->Yield();
}
