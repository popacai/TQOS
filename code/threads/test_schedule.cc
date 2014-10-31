#include "test_schedule.h"

int low = 0;
int medium = 0;
int high = 0;

void lock_thread(int args) {
    Lock* lock = (Lock*) args;
    lock->Acquire();
    for (int i = 0; i < 10; i++) {
        printf("%s, %d\n", currentThread->getName(), i);
        printf("%s, %d\n", currentThread->getName(), currentThread->getPriority());
        if (strcmp(currentThread->getName(), "low") == 0) {
            low++;
        }
        if (strcmp(currentThread->getName(), "medium") == 0) {
            medium++;
        }
        if (strcmp(currentThread->getName(), "high") == 0) {
            high++;
        }

        // Checking status
        if (low == 10) {
            ASSERT(high == 0);
            ASSERT(medium == 0);
        }
        if (medium == 10) {
            ASSERT(low == 10);
            ASSERT(high == 0);
        }
        currentThread->Yield();
    }
    lock->Release();
}
int test_priority_lock() {
    Lock* lock = new Lock("lock");
    int args = (int)lock;
    Thread* t_low = new Thread("low", 1);
    t_low->setPriority(10);
    t_low->Fork(lock_thread, args);
    Thread* t_high = new Thread("high", 1);
    t_low->setPriority(30);
    t_high->Fork(lock_thread, args);

    t_low->Join();
    t_high->Join();
    printf("SUCCESS\n");
    return 0;
}

int test_priority_cond() {
    return 0;
}

int test_priority_cv() {
    return 0;
}

int test_inherient_lock() {
    Lock* lock = new Lock("lock");
    int args = (int)lock;
    Thread* t_low = new Thread("low", 1);
    t_low->setPriority(10);
    t_low->Fork(lock_thread, args);

    Thread* t_medium = new Thread("medium", 1);
    t_medium->setPriority(20);
    t_medium->Fork(lock_thread, args);

    Thread* t_high = new Thread("high", 1);
    t_high->setPriority(30);
    t_high->Fork(lock_thread, args);

    t_low->Join();
    t_medium->Join();
    t_high->Join();
    printf("SUCCESS\n");
}

int test_complex_inherient_lock() {
    return 0;
}
