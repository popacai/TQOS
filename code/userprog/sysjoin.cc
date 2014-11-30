#include "ksyscall.h"
#include "system.h"
#include "thread.h"
#include "syscommon.h"
#include "addrspace.h"
#include "processmanager.h"

void kjoin() {
    int joineeId;
    joineeId = machine->ReadRegister(4);
    Thread * joineeThread;
    joineeThread = (Thread*)(processManager->Get(joineeId));
    joineeThread -> Join();
    PushPC();
}
