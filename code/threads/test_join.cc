#include "test_join.h"

void run_thread_child(int args){
    int i;
    for(i = args; i > 0; i--) {
        currentThread->Yield();
        printf("%d   In child thread execution %d .\n", args, i);
        printf("%d current priority: %d\n", args, currentThread->getPriority());
    }
    printf("%d Child Finish!\n", args);
}

int test_join(){
    currentThread->setPriority(30);
    Thread* mid = new Thread("mid priority thread");
    mid->setPriority(20);
    mid->Fork(run_thread_child, 20);
    Thread* mid2 = new Thread("mid priority thread");
    mid2->setPriority(25);
    mid2->Fork(run_thread_child, 25);
    Thread* child= new Thread("child thread", 1); // enable join
    child->setPriority(1); // low priority child
    child->Fork(run_thread_child, 1); 
    printf("in parent thread\n"); 

    child->Join();
    printf("%d Parent Exits: Test Finish! \n", 30);
    return 1;
}    

int test_destroy_after_join(){
    Thread * child = new Thread("child thread", 1);
    child->Fork(run_thread_child, 10); // yield 10 times
    
}
