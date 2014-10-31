#include "test_schedule.h"

int low = 0;
int medium = 0;
int high = 0;

void lock_thread(int args) {
    Lock* lock = (Lock*) args;
    printf("%s, run\n", currentThread->getName());
    lock->Acquire();
    if (strcmp(currentThread->getName(), "low") == 0) {
        low++;
        ASSERT(high == 1);
    }
    if (strcmp(currentThread->getName(), "high") == 0) {
        high++;
        ASSERT(low == 0);
    }
    lock->Release();
}
int test_priority_lock() {
    Lock* lock = new Lock("lock");
    int args = (int)lock;
    lock->Acquire();

    Thread* t_low = new Thread("low", 1);
    t_low->setPriority(10);
    t_low->Fork(lock_thread, args);
    currentThread->Yield();

    Thread* t_high = new Thread("high", 1);
    t_high->setPriority(30);
    t_high->Fork(lock_thread, args);
    currentThread->Yield();
    lock->Release();

    t_low->Join();
    t_high->Join();
    printf("SUCCESS\n");
    return 0;
}

void wait_cond(int args) {
    int* arg = (int*)args;
    Lock* lock = (Lock*) arg[0];
    Condition* cond = (Condition*) arg[1];

    printf("%s, run\n", currentThread->getName());
    lock->Acquire();
    cond->Wait(lock);

    currentThread->Yield();
    currentThread->Yield();
    currentThread->Yield();
    currentThread->Yield();
    if (strcmp(currentThread->getName(), "high") == 0) {
        high++;
        ASSERT(low == 0);
    }
    if (strcmp(currentThread->getName(), "low") == 0) {
        low++;
        ASSERT(high == 1);
    }
    currentThread->Yield();
    lock->Release();
    printf("%s, exit\n", currentThread->getName());
}

int test_priority_cond() {
    Lock* lock = new Lock("lock");
    Condition* cond = new Condition("cond");

    int* args = new int[2];
    args[0] = (int)lock;
    args[1] = (int)cond;
    int arg = (int) args;

    Thread* t_low = new Thread("low", 1);
    t_low->setPriority(10);
    t_low->Fork(wait_cond, arg);
    currentThread->Yield();

    Thread* t_high = new Thread("high", 1);
    t_high->setPriority(30);
    t_high->Fork(wait_cond, arg);
    currentThread->Yield();

    lock->Acquire();
    cond->Signal(lock);  //signal cond. expect high to run
    lock->Release();

    t_high->Join();
    
    lock->Acquire();
    cond->Signal(lock);  //signal cond. expect low to run
    lock->Release();

    t_low->Join();

    printf("SUCCESS\n");
    return 1;
}

int test_priority_broadcast() {
    Lock* lock = new Lock("lock");
    Condition* cond = new Condition("cond");

    int* args = new int[2];
    args[0] = (int)lock;
    args[1] = (int)cond;
    int arg = (int) args;

    Thread* t_low = new Thread("low", 1);
    t_low->setPriority(10);
    t_low->Fork(wait_cond, arg);
    currentThread->Yield();

    Thread* t_medium = new Thread("medium", 1);
    t_medium->setPriority(20);
    t_medium->Fork(wait_cond, arg);
    currentThread->Yield();

    Thread* t_high = new Thread("high", 1);
    t_high->setPriority(30);
    t_high->Fork(wait_cond, arg);
    currentThread->Yield();

    lock->Acquire();
    cond->Broadcast(lock);  //signal cond. expect low to run
    lock->Release();


    t_low->Join();
    t_high->Join();
    printf("SUCCESS\n");
    return 1;
 
}

void P(int args) {
    printf("%s, run\n", currentThread->getName());
    Semaphore* s = (Semaphore*) args;
    currentThread->Yield();
    s->P();
    if (strcmp(currentThread->getName(), "high") == 0) {
        high++;
        ASSERT(medium == 0);
        ASSERT(low == 0);
    }

    if (strcmp(currentThread->getName(), "medium") == 0) {
        medium++;
        ASSERT(high == 1);
        ASSERT(low == 0);
    }

    if (strcmp(currentThread->getName(), "low") == 0) {
        low++;
        ASSERT(medium == 1);
        ASSERT(high == 1);
    }
    currentThread->Yield();
    printf("%s, exit\n", currentThread->getName());
}

int test_priority_cv() {
    Semaphore* s = new Semaphore("semaphore", 0);
    int args = (int)s;

    Thread* t_low = new Thread("low", 1);
    t_low->setPriority(10);
    t_low->Fork(P, args);
    currentThread->Yield();

    Thread* t_medium = new Thread("medium", 1);
    t_medium->setPriority(20);
    t_medium->Fork(P, args);
    currentThread->Yield();

    Thread* t_high = new Thread("high", 1);
    t_high->setPriority(30);
    t_high->Fork(P, args);
    currentThread->Yield();

    s->V();
    s->V();
    s->V();

    t_low->Join();
    t_medium->Join();
    t_high->Join();
    printf("SUCCESS\n");

    return 1;
}

void loop_10_times(int args) {
    Lock* lock = (Lock*) args;
    lock->Acquire();
    for (int i = 0; i < 10; i++) {
        printf("%s, %d\n", currentThread->getName(), i);
        printf("%s, %d\n", currentThread->getName(), currentThread->getPriority());
        currentThread->Yield();
    }
    lock->Release();
    if (strcmp(currentThread->getName(), "high") == 0) {
        high++;
        ASSERT(medium == 0);
        ASSERT(low == 1);
    }
    if (strcmp(currentThread->getName(), "low") == 0) {
        low++;
        ASSERT(high == 0);
        ASSERT(medium == 0);
    }
    if (strcmp(currentThread->getName(), "medium") == 0) {
        medium++;
        ASSERT(high == 1);
        ASSERT(low == 1);
    }

    printf("%s, exit\n", currentThread->getName());
}

void loop_10_times_no_lock(int args) {
    for (int i = 0; i < 10; i++) {
        printf("%s, %d\n", currentThread->getName(), i);
        printf("%s, %d\n", currentThread->getName(), currentThread->getPriority());
        currentThread->Yield();
    }

    if (strcmp(currentThread->getName(), "medium") == 0) {
        medium++;
        ASSERT(high == 1);
        ASSERT(low == 1);
    }
    if (strcmp(currentThread->getName(), "low") == 0) {
        low++;
        ASSERT(high == 0);
        ASSERT(medium == 0);
    }
    if (strcmp(currentThread->getName(), "high") == 0) {
        high++;
        ASSERT(medium == 0);
        ASSERT(low == 1);
    }

    printf("%s, exit\n", currentThread->getName());
}
int test_inherient_lock() {
    Lock* lock = new Lock("lock");
    int args = (int)lock;

    Thread* t_low = new Thread("low", 1);
    t_low->setPriority(0);
    t_low->Fork(loop_10_times, args);
    currentThread->Yield();

    Thread* t_medium = new Thread("medium", 1);
    t_medium->setPriority(20);
    t_medium->Fork(loop_10_times_no_lock, args);
    
    Thread* t_high = new Thread("high", 1);
    t_high->setPriority(30);
    t_high->Fork(loop_10_times, args);

    t_medium->Join();
    t_low->Join();
    t_high->Join();

    printf("SUCCESS\n");
    return 1;
}


int test_complex_inherient_lock() {

}
