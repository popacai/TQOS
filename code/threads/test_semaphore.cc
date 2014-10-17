#include "test_semaphore.h"

void run_thread_v(int args){
    // execute semaphore V()
    Semaphore* semaphore = (Semaphore*) args;
    for (int i=0; i<2; i++){
        semaphore->V();
        printf("currentThread=%s is %s,%d\n", currentThread->getName(),"producer",semaphore->getValue());
    }
    currentThread->Yield();
    semaphore->V();
    printf("currentThread=%s is %s, %d\n", currentThread->getName(),"producer",semaphore->getValue());
    currentThread->Yield();
}

void run_thread_p(int args){
    // execute semaphore P()
    Semaphore* semaphore = (Semaphore*) args;
    for (int i=0; i<3; i++){
        semaphore->P();
        printf("currentThread=%s is %s,%d\n", currentThread->getName(),"consumer",semaphore->getValue());
    }
    printf("Test Finish!");
}

int test_semaphore(){
    Semaphore* semaphore1 = new Semaphore("simplesemaphore",0);

    Thread *t1 = new Thread("t1");
    t1->Fork(run_thread_v, (int)semaphore1);

    Thread *t2 = new Thread("t2");
    t2->Fork(run_thread_p, (int)semaphore1);
    return 1;
}    
