#include "test_join.h"

void run_thread_child(int args){
    int i;
    for(i = args; i > 0; i--) {
        printf("In child thread execution %d .\n", i);
    }
    printf("Child Finish!\n");
    currentThread->Yield();
}

int test_join(){
    Thread* child= new Thread("child thread", 1); // enable join
    child->Fork(run_thread_child, 10); 
    printf("in parent thread\n"); 

    child->Join();
    printf("Parent Exits: Test Finish! \n");
    return 1;
}    
