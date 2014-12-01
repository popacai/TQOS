#include "ksyscall.h"
#include "system.h"
#include "thread.h"
#include "syscommon.h"
#include "addrspace.h"
#include "processmanager.h"

void
StartUserProcess(int argv) {
    if(argv != 0) {
        char** _argv = (char**)argv;
        printf("argc = %d\n", (int)_argv[0]);
    }
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();
    machine->Run();
    // TODO: copy argv data to user space & set register
    ASSERT(false);
}

void kexec() {
    int srcPath, len, argc, argv, opt;
    unsigned char ** passArgv;
    unsigned char * path;
    Thread * t;
    OpenFile * executable;
    AddrSpace * space;
    int spid;
    //printf("Thread name that call EXEC: %s\n", currentThread->getName()); 
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
        ASSERT(false);
    }
    //printf("user str : %s, len: %d \n", path, len);
    if(argc == 0) {
        passArgv = NULL;
    }
    else {
        passArgv = new unsigned char*[argc + 1]; // store argc first
        passArgv[0] = (unsigned char*)argc;
        u2kmatrixcpy(passArgv+4, argv, argc);
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
    delete executable;
    delete path;
    spid = processManager->Alloc((void*)t);
    machine->WriteRegister(2, spid);
    if(spid == 0) {// not enough spid in process manager
        machine->WriteRegister(2, 0); // return err code 0
        delete t;
        PushPC();
        return;
    }
    //printf("new spid: %d\n", spid);
    t->spid = spid;
    t->Fork(StartUserProcess, (int)passArgv); 
    currentThread->Yield(); 
    PushPC();
}
