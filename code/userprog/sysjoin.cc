#include "ksyscall.h"
#include "system.h"
#include "thread.h"
#include "syscommon.h"
#include "addrspace.h"
#include "processmanager.h"

void kjoin() {
    int joineeId;
    joineeId = machine->ReadRegister(4);
    ASSERT(joineeId > 0);
    Thread * joineeThread;
    joineeThread = (Thread*)(processManager->Get(joineeId));
    ASSERT(joineeThread->join > 0); // able to join
    joineeThread -> Join();
    PushPC();
}
