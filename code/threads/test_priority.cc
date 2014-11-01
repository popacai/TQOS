#include "test_priority.h"

void run_child(int args){
    int i;
    for(i = 10; i > 0; i--) {
        printf("In child thread execution %d .\n", args);
    currentThread->Yield();
    }
    printf("Child Finish!\n");
}

int test_priority(){
    currentThread->setPriority(100);
    Thread* child1= new Thread("child thread"); 
    child1->setPriority(10);
    child1->Fork(run_child, child1->getPriority()); 
    Thread* child2= new Thread("child thread"); 
    child2->setPriority(30);
    child2->Fork(run_child, child2->getPriority()); 
    printf("in parent thread\n"); 
    currentThread->Yield();
    printf("Parent Exits: Test Finish! \n");
    return 1;
}    
