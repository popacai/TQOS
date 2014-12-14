#include "test_join.h"

void run_thread_child(int args) {
    int i;
    for(i = args; i > 0; i--) {
        currentThread->Yield();
        printf("%d   In child thread execution %d .\n", args, i);
        printf("%d current priority: %d\n", args, currentThread->getPriority());
    }
    printf("%d Child Finish!\n", args);
}

void join_oneself(int args) {
    currentThread->Yield();
    currentThread->Join();
}

int test_join() {
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

int test_destroy_after_join() {
    Thread * child = new Thread("child thread", 1);
    child->Fork(run_thread_child, 10); // yield 10 times
    child->Join();
    for(int i = 0; i < 3; i++) {
        printf("should be true: %d\n", threadToBeDestroyed != child);
        printf("in parent thread\n");
        currentThread->Yield();
    }
    return 1;
}

int test_child_finish_before_join() {
    Thread * child = new Thread("child thread", 1);
    child->setPriority(10); // higher than parent
    child->Fork(run_thread_child, 15);
    currentThread->Yield();
    child->Join();
    printf("parent finish\n");
    return 1;
}

int test_cannot_join_itself() {
    Thread * child = new Thread("child thread", 1);
    child->Fork(join_oneself, 1);
    printf("SHOULD BE ASSERTION FAIL\n");
    return 1;
}

int test_join_notjoinable_thread() {
    printf("SHOULD BE ASSERTION FAIL\n");
    Thread * child = new Thread("child thread");
    child->Fork(run_thread_child, 1);
    child->Join();
    return 1;
}

int test_join_must_fork() {
    printf("SHOULD BE ASSERTION FAIL\n");
    Thread * child = new Thread("child thread", 1);
    child->Join();
    return 1;
}

int test_must_not_join_twice() {
    printf("SHOULD BE ASSERTION FAIL\n");
    Thread * child = new Thread("child thread", 1);
    child->Fork(run_thread_child, 10);
    child->Join();
    child->Join();
    return 1;
}
