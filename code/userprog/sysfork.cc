#include "thread.h"
#include "syscommon.h"
#include "ksyscall.h"
#include "machine.h"
#include "addrspace.h"

void start_child_thread(int func_ptr) {
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();		// load page table register

    machine->WriteRegister(PCReg, func_ptr - 4);
    machine->WriteRegister(NextPCReg, func_ptr);

    //If the child thread return, jump back to Exit System Call
    // machine->WriteRegister(RetAddrReg, 0x10 + 4 * 3);
    // jump back to the main exit jal main. in start.cc
    machine->WriteRegister(RetAddrReg, 4 * 2);
    machine->Run();			// jump to the user progam
    ASSERT(false);
}

Thread* find_next_thread() {
    Thread* previous = currentThread;
    Thread* next = currentThread->nextThread;
    while (next != currentThread) {
        previous = next;
        next = next->nextThread;
    }
    return previous;
}
    
int kfork(int func_ptr) {
    Thread* t;
    Thread* the_end_of_currentThread;
    AddrSpace* new_space;
    int currentPC, currentNextPC;


    new_space = new AddrSpace(); //a new space
    t = new Thread("child thread");

    if (new_space->CopyCurrentSpace()) {
        return -1;
    }

    //DEBUG for space
    t->space = new_space;

    the_end_of_currentThread = find_next_thread();
    the_end_of_currentThread->nextThread = t;
    t->nextThread = currentThread;

    //t->SaveUserState();
    t->spid = currentThread->spid;

    t->Fork(start_child_thread, func_ptr);

    currentThread->Yield();
    return 0;
}
