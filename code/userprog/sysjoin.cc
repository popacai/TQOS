#include "ksyscall.h"
#include "system.h"
#include "thread.h"
#include "syscommon.h"
#include "addrspace.h"
#include "processmanager.h"

void kjoin() {
    int joineeId;
    int exitStatus;
    joineeId = machine->ReadRegister(4);
    Thread * joineeThread;
    joineeThread = (Thread*)(processManager->Get(joineeId));
    joineeThread -> Join();
    exitStatus = joineeThread->exitStatusCode;
    machine->WriteRegister(2, exitStatus); // return exit code
    PushPC();
}
