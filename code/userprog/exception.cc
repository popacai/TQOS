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
void
StartUserProcess(int argv)
{
    // TODO: use test file path, should pass in filename
    char * filename;
    int * _argv = (int *) argv;
    int argc = _argv[0];
    if(argc < 1) {
        ASSERT(false);
    }
    if(argc == 1) {
        // shallow copy because no need to keep the value
        filename = (char*)_argv[1];
        printf("pass in file path: %s\n", filename);
    }
    if(argc > 1) {
        // for now
        ASSERT(false);
        // TODO: add more argv without checking, checked before passed here
    }
    OpenFile *executable = fileSystem->Open(filename);
    AddrSpace *space;

    if (executable == NULL) {
        printf("Unable to open file %s\n", filename);
        return;
    }
    space = new AddrSpace();
    if(space->Initialize(executable)) { // use locks
        currentThread->space = space;
    }
    else {
        // TODO: return error code & handle error
    }

    delete executable;			// close file

    space->InitRegisters();		// set the initial register values
    space->RestoreState();		// load page table register

    machine->Run();			// jump to the user progam
    ASSERT(FALSE);			// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
}

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
     unsigned int num_pages;
     char* thread_name;
     AddrSpace *space;
     space = currentThread->space;
     thread_name = currentThread->getName();
     printf("kernel level thread name = %s\n",thread_name);
     num_pages = space->getNumPages();
     printf("numPages = %d\n",num_pages);
     currentThread->Finish();
     printf("after finish\n");
     return 1;
}

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    int buffer, size, id;
    int errno;
    unsigned char * path;
    int srcPath, len, argc, argv, opt;
    int * passArgv;
    //int arg_vaddr[4] = {0};

    Thread * t;
    int spid;
    if (which == SyscallException) {
        switch (type) {
            case SC_Halt:
                DEBUG('a', "Shutdown, initiated by user program.\n");
                printf("%s\n", currentThread->getName());
                printf("halt\n");
                interrupt->Halt();
                break;

            case SC_Exit:
                printf("%s\n", currentThread->getName());
                printf("exit\n");
                int sp;

                //currentThread->Yield();
                currentThread->Finish();
                //interrupt->Halt();
                return;
                break;

            case SC_Exec:
                printf("exec\n"); 
                srcPath = machine->ReadRegister(4);
                argc = machine->ReadRegister(5);
                argv = machine->ReadRegister(6);
                opt = machine->ReadRegister(7);
                // first check the location of filename is valid
                if(!fname_addrck((char*)srcPath)){
                    ASSERT(false);
                }
                len = ustrlen((int)srcPath);
                path = new unsigned char[len+1];
                u2kmemcpy(path, srcPath, len + 1);
                if (fexist_ck(path) == -1) {
                //    ASSERT(false);
                }
                printf("user str : %s, len: %d  %c\n", path, len, path[len]);
                if(argc < 1) {
                    ASSERT(false);
                }
                if(argc == 1) {
                    // only file path
                    passArgv = new int[2];
                    passArgv[0] = argc;
                    passArgv[1] = (int)path;
                }
                else {
                    // for now
                    ASSERT(false);
                    // TODO: check argv
                }
                t = new Thread("exec new thread");
                t -> Fork(StartUserProcess, (int)passArgv); // use fake arg
                spid = processManager->Alloc((void*)t);
                // TODO: handle when there is no spid
                PushPC();
                machine->WriteRegister(2, spid);
                printf("new spid: %d\n", spid);
                currentThread->Yield(); 
                break;

            case SC_Fork:
                printf("fork\n");
                // interrupt->Halt();
                break;

            case SC_Yield:
                //printf("yield\n");
                currentThread->Yield();
                PushPC();
                //machine->Run();
                
                //interrupt->Halt();
                break;
            
            case SC_Read:
                buffer = machine->ReadRegister(4);
                size = machine->ReadRegister(5);
                id = machine->ReadRegister(6);

                errno = RW_bufck(buffer, size, id);
                if (errno < 0) {    
                    printf("error.\n");
                    kill_process();
                }

                kread((char*)buffer, size, id);

                PushPC();
                break;

            case SC_Write:
                buffer = machine->ReadRegister(4);
                size = machine->ReadRegister(5);
                id = machine->ReadRegister(6);

                errno = RW_bufck(buffer, size, id);
                if (errno < 0) {    
                    printf("error.\n");
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
        ASSERT(false);
    }
    else if (which == IllegalInstrException) {
        //TODO handle illegal instruction
        printf("illegal instruction exception\n");
        ASSERT(false);
    } 
    else if (which == OverflowException) {
        //TODO handle interger overflow
        printf("Unexpected user mode exception %d %d\n", which, type);
        ASSERT(false);
    }
    else if (which == AddressErrorException) {
        //TODO handle address error 
        printf("Unexpected user mode exception %d %d\n", which, type);
        ASSERT(false);
    } else {
        printf("Unexpected user mode exception %d %d\n", which, type);
        ASSERT(FALSE);
    }
}
