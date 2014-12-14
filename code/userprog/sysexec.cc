#include "ksyscall.h"
#include "system.h"
#include "thread.h"
#include "syscommon.h"
#include "addrspace.h"
#include "processmanager.h"

#define PATH_MAX 64

void
StartUserProcess(int argv) {
    int i;
    int argc = 0;
    int argvStartAddr;
    int argvPtrAddr;
    int len;
    char** _argv;
    if(argv != 0) {
        _argv = (char**)argv;
        argc = (int)_argv[0];
    }
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();
    if(argv != 0) {
        argvStartAddr = machine->ReadRegister(StackReg) + 16 - UserStackSize;
        argvPtrAddr = machine->ReadRegister(StackReg) + 16 - UserStackSize + ArgvSize; // used to store pointers
        for(i = 1; i <= argc; i++) {
            len = strlen(_argv[i]) + 1;
            k2umemcpy(argvStartAddr, (unsigned char*)_argv[i], len);
            k2umemcpy(argvPtrAddr, (unsigned char*)(&argvStartAddr), 4);
            argvStartAddr += len;
            argvPtrAddr += 4;
        }
        argvPtrAddr = machine->ReadRegister(StackReg) + 16 - UserStackSize + ArgvSize;
        machine->WriteRegister(4, argc);
        machine->WriteRegister(5, argvPtrAddr);
        // delete
        for(i = 1; i <= argc; i++) {
            delete _argv[i];
        }
        delete _argv;
    }
    machine->Run();

    ASSERT(false);
}

void kexec() {
    int srcPath, len, argc, argv, opt;
    unsigned char ** passArgv;
    unsigned char * path;
    unsigned char * buffer;
    Thread * t;
    OpenFile * executable;
    AddrSpace * space;
    int spid;
    int buf_size;
    int num_pages;
    unsigned long task_size;
    //printf("Thread name that call EXEC: %s\n", currentThread->getName());
    srcPath = machine->ReadRegister(4);
    argc = machine->ReadRegister(5);
    argv = machine->ReadRegister(6);
    opt = machine->ReadRegister(7);
    // first check the location of filename is valid
    if(argc < 0 || opt < 0 || opt > 0x111b) {
        printf("argc or opt invalid! \n");
        machine->WriteRegister(2, 0); // return err code 0
        PushPC();
        return;
    }
    // check file name memory addr 
    num_pages = machine->pageTableSize;
    task_size = num_pages * PageSize;
    if((unsigned long)srcPath > task_size) {
        printf("file name memory addr invalid!\n");
        machine->WriteRegister(2, 0); // return err code 0
        PushPC();
        return;
    }
    if(task_size - (unsigned long)srcPath < PATH_MAX) {
        // adjust len to max size
        len = task_size - (unsigned long)srcPath;
    }
    else {
        len = PATH_MAX;
    }
    
    // check file name 
    buffer = new unsigned char[len + 2];
    u2kmemcpy(buffer, srcPath,  len + 1);
    buffer[len + 1] = '\0';
    buf_size = strlen((char*)buffer);
    //printf("buf size: %d\n", buf_size);
    if(buf_size < 1 || buf_size > len) {
        delete buffer;
        printf("file path invalid\n");
        machine->WriteRegister(2, 0); // return err code 0
        PushPC();
        return;
    }
    path = new unsigned char[buf_size + 1];
    strcpy((char*)path, (char*)buffer);
    delete buffer;
    
    // check file exist
    if(!fexist_ck(path)) {
        printf("file does not exist\n");
        delete path;
        machine->WriteRegister(2, 0); // return err code 0
        PushPC();
        return;
    }
   
    // in P3, just support argc == 0
    if(argc == 0) {
        passArgv = NULL;
    }
    else {
        passArgv = new unsigned char*[argc + 1]; // store argc first
        passArgv[0] = (unsigned char*)argc;
        u2kmatrixcpy(&passArgv[1], argv, argc);
        // TODO: remember to free memory...
    }
    executable = fileSystem->Open((char*)path);
    space = new AddrSpace();
    t = new Thread("exec new thread", opt);
    if(space->Initialize(executable)) {
        t -> space = space;
    }
    else {
        machine->WriteRegister(2, 0); // return err code 0
        delete t;
        PushPC();
        return;
        ASSERT(false); // should not reach here
    }
    //delete executable;
    //delete path;
    spid = processManager->Alloc((void*)t);
    machine->WriteRegister(2, spid);
    if(spid == 0) {// not enough spid in process manager
        machine->WriteRegister(2, 0); // return err code 0
        delete t;
        PushPC();
        return;
    }
    space->backstore = new BackStore(space, spid);
    //printf("new spid: %d\n", spid);
    t->spid = spid;
    t->Fork(StartUserProcess, (int)passArgv);
    currentThread->Yield();
    PushPC();
}
