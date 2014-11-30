// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscommon.h"
#include "syscall.h"
#include "thread.h"
#include "ksyscall.h"
#include "processmanager.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------

void k_exec(int arg_vaddr[]) {
    unsigned char* name;
    int len, k_argc, k_opt;
    unsigned char** k_argv;
    int errno;

    // first check the location of filename is valid
    errno = fname_addrck((char*)arg_vaddr[0]);
    //printf("errno = %d\n",errno);
    if (errno <= 0) {    
        ASSERT(false);
    }
    len = ustrlen(arg_vaddr[0]);
    name = new unsigned char[len];
    u2kmemcpy(name,arg_vaddr[0],len);
    printf("name = %s\n",name);
    if (fexist_ck(name) == -1) {
        ASSERT(false);
    }
    k_argc = arg_vaddr[1];
    printf("argc value=%d\n",k_argc);
    k_opt = arg_vaddr[3];
    printf("opt value=%d\n",k_opt);

    k_argv = new unsigned char*[k_argc];
    u2kmatrixcpy(k_argv,arg_vaddr[2],k_argc);
    for (int i = 0; i< k_argc; i++) {
        printf("argv[%d] value = %s\n",i,k_argv[i]);
    }
}

int kill_process() {
/*     unsigned int num_pages;
     char* thread_name;
     AddrSpace *space;
     space = currentThread->space;
     thread_name = currentThread->getName();
     printf("kernel level thread name = %s\n",thread_name);
     num_pages = space->getNumPages();
     printf("numPages = %d\n",num_pages);*/
     delete currentThread->space; // memory manager
     processManager->Release(currentThread->spid); // process manager
     currentThread->Finish();
     return 1;
}

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    int buffer, size, id, arg1;
    int errno;
    int read_return_value;
    int name, argc_, argv_, opt;    
    int inargv;
    int i;
    unsigned char* path;

    if (which == SyscallException) {
        switch (type) {
            case SC_Halt:
                DEBUG('a', "Shutdown, initiated by user program.\n");
                printf("halt\n");
                interrupt->Halt();
                break;

            case SC_Exit:
                // TODO: should be moved to sysexit.cc like Exec 
                //printf("Exit Thread Name %s\n", currentThread->getName());
                //printf("exit %d\n", currentThread->spid);
                if(currentThread->spid != 1) {
                    // if not main thread, just finish
                    // free resource before finish
                    //printf("spid=%d\n",currentThread->spid);
                    kill_process();
                }
                else {
                    ASSERT(currentThread->spid == 1);
                    while(1 != processManager->TestForExit()) {
                        // if there are other threads running, yield
                        currentThread->Yield();
                    } 
                    // when there's only me, halt
                    // TODO: if not halt here, release resource? 
                    //printf("see you! \n");
                    interrupt->Halt();
                }
                break;

            case SC_Exec:
                name = machine->ReadRegister(4);
                argc_ = machine->ReadRegister(5);
                argv_ = machine->ReadRegister(6);
                opt = machine->ReadRegister(7);
                if (fname_addrck((char*)name) <= 0) {
                    printf("name error\n");
                    PushPC();
                    break;
                    //kill_process();
                }else if ((argc_ < 0) || (RW_bufck(argv_, argc_) <= 0) ) { 
                    //here the RW_bufck is just used for check the argv_ address, no work with read/write
                    printf("argc or argv error\n");
                    PushPC();
                    break;
                    //kill_process();
                }
                else if (opt < 0 || opt > 0x111b) {
                    printf("wrong opt\n");
                    PushPC();
                    break;
                }
                size = ustrlen(name);
                path = new unsigned char[size+1];
                u2kmemcpy(path, name, size+1);
                if (fexist_ck(path) <= 0) {
                    printf("name not exits\n");
                    PushPC();
                    break;
                }
/*                for (i = 0; i < argc_; i++) {
                    machine->ReadMem(argv_ + i*4, 4, &inargv);
                    errno = fname_addrck((char*) inargv);
                    if (errno <= 0 ) {
                        printf("inargv error\n");
                        PushPC();
                        break;
                        //kill_process();
                    }
                } 
*/                kexec();
                break;

            case SC_Join:
                kjoin();
                break;

            case SC_Fork:
                printf("fork\n");
                arg1 = machine->ReadRegister(4);
                kfork(arg1);
                PushPC();
                // interrupt->Halt();
                break;

            case SC_Yield:
                //printf("yield\n");
                currentThread->Yield();
                PushPC();
                break;
            
            case SC_Read:
                buffer = machine->ReadRegister(4);
                size = machine->ReadRegister(5);
                id = machine->ReadRegister(6);

                errno = RW_bufck(buffer, size);
                if (errno < 0) {    
                    printf("error.\n");
                    kill_process();
                }
                if (id != ConsoleInput) {
                    printf("error id.\n");
                    kill_process();
                }
                read_return_value = kread((char*)buffer, size, id);
                machine->WriteRegister(2, read_return_value);

                PushPC();
                break;

            case SC_Write:
                buffer = machine->ReadRegister(4);
                size = machine->ReadRegister(5);
                id = machine->ReadRegister(6);

                errno = RW_bufck(buffer, size);
                if (errno < 0) {    
                    printf("%d\n",errno);
                    printf("error.\n");
                    kill_process();
                }
                if (id != ConsoleOutput) {
                    printf("error id.\n");
                    kill_process();
                }

                kwrite((char*)buffer, size, id);

                PushPC();
                break;

            default:
                printf("Unexpected user mode exception %d %d\n", which, type);
                //interrupt->Halt();
                ASSERT(FALSE);
        }
    }
    else if (which == NumExceptionTypes) {
        //TODO handle arithematic exception
        printf("number exception\n");
        kill_process();
    }
    else if (which == IllegalInstrException) {
        //TODO handle illegal instruction
        printf("illegal instruction exception\n");
        kill_process();
    } 
    else if (which == OverflowException) {
        //TODO handle interger overflow
        printf("Unexpected user mode exception %d %d\n", which, type);
        kill_process();
    }
    else if (which == AddressErrorException) {
        //TODO handle address error 
        printf("Unexpected user mode exception %d %d\n", which, type);
        kill_process();
    } else {
        printf("Unexpected user mode exception %d %d\n", which, type);
        kill_process();
    }
}
