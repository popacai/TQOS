#include "test_semaphore.h"

void produce(int arg){
    int* args = (int*) arg;
    
    Semaphore* semaphore = (Semaphore*) args;
    
    for (int i=0; i<2; i++){
        semaphore->V();
        printf("currentThread=%s,%d\n", currentThread->getName(), semaphore->getvalue());
    }

    currentThread->Yield();
    
    semaphore->V();
    printf("currentThread=%s,%d\n", currentThread->getName(), semaphore->getvalue());

    currentThread->Yield();    
}

void consume(int arg){    
    int* args = (int*) arg;

    Semaphore* semaphore = (Semaphore*) args;

    for (int i=0; i<3; i++){
        semaphore->P();
        printf("currentThread=%s,%d\n", currentThread->getName(), semaphore->getvalue());
    }
    
    currentThread->Yield();
}

int test_semaphore(){
    Semaphore *semaphore = new Semaphore("simplesemaphore", 0);
    int args = (int)semaphore;
    
    Thread* t= new Thread("producer");
    t->Fork(produce, args);
   
    t = new Thread("consumer");
    t->Fork(consume, args);
    return 1;
}
