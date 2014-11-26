#include "ksyscall.h"
#include "system.h"
#include "thread.h"
#include "syscommon.h"
#include "addrspace.h"
#include "processmanager.h"

void
StartUserProcess(int argv) {
    currentThread->space->InitRegisters();
    currentThread->space->RestoreState();
    machine->Run();
    ASSERT(false);
}

void kexec() {
    int srcPath, len, argc, argv, opt;
    int * passArgv;
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
        passArgv = NULL; //TODO: handle passing argv
    }
    executable = fileSystem->Open((char*)path);
    space = new AddrSpace();
    t = new Thread("exec new thread", opt);
    if(space->Initialize(executable)) {
        t -> space = space;
    }
    else {
        //TODO: return error, not assert
        ASSERT(false);
    }
    delete executable;
    spid = processManager->Alloc((void*)t);
    // TODO: handle when there is no spid
    machine->WriteRegister(2, spid);
    //printf("new spid: %d\n", spid);
    t->spid = spid;
    t -> Fork(StartUserProcess, (int)passArgv); 
    currentThread->Yield(); 
    PushPC();
}
