#include "test_lock.h"


void release_not_held_lock(int args) {
    Lock* lock = (Lock*) args;
    printf("Current lock name is %s\n",lock->getName());
    printf("Current thread name is %s\n",currentThread->getName());
    if (!lock->isHeldByCurrentThread()){
        printf("release a lock not acquire by both threads!\n");
        lock->Release();
    }
    //currentThread->Yield();   
}

void test_release_not_held() {
    Lock* lock = new Lock("lock_not_held");
    Thread* t = new Thread("t3");
    printf("Current lock name is %s\n",lock->getName());
    printf("Current thread name is %s\n",currentThread->getName());    
    release_not_held_lock((int)lock);
    printf("Process B release a lock have never been held\n");
    lock->Acquire();
    printf("Process A has already aquire this lock\n");   // test for some thread first release the lock
    t->Fork(release_not_held_lock, (int)lock);
    // The process B has release the lock, since the queue is empty for the lock, it will successfully acquire twice for Process A
    currentThread->Yield();
    // If no Yield(), the parents will acquire the lock twice and append to the queue
    // The current code in Lock::Release will check "ASSERT" the holder of lock, Process B cannot release  
    lock->Acquire();
    printf("Process A aquires the same lock twice\n");
    currentThread->Yield();
}    


void runThread(int args) {
    Lock* lock = (Lock*) args;
    char* name = lock->getName();
    // DEBUG('t', "ASSERT NAME");
    printf("ASSERT NAME %s\n", name);
    ASSERT(strcmp(name, "simplelock") == 0);
}

int test_pass_arguments() {
    Lock *lock1 = new Lock("simplelock");
    Thread *t = new Thread("t1");


    t->Fork(runThread, (int)lock1);
    runThread((int)lock1);
    return 1;
}

void lock_and_increase(int arg) {
    int* args = (int*) arg;

    Lock* lock = (Lock*) args[0];
    int* value = (int*) args[1];

    int first_value_in_loop;
    int second_value_in_loop;

    for (int i = 0; i < 10; i++) {
        lock->Acquire();
        (*value)++;
        // printf("currentThread=%s,%d\n", currentThread->getName(), *value);
        first_value_in_loop = *value;

        currentThread->Yield();
        (*value)++;
		// printf("currentThread=%s,%d\n", currentThread->getName(), *value);
        second_value_in_loop = *value;
        ASSERT(second_value_in_loop == first_value_in_loop + 1);

        lock->Release();
        currentThread->Yield();
    }
}

int test_single_lock() {
    /*
     * Create 3 threads. which may yield in / out the critical section.
     * Without the lock. The value will be increased by two by different threads
     * With the lock. The value will be increased by two by one thread
     */

    Lock *lock = new Lock("simplelock");
    int* value = new int;

    int* args = new int[2];
    args[0] = (int)lock;
    args[1] = (int)value;

    Thread* t;
    t = new Thread("t1");
    t->Fork(lock_and_increase, (int)args);

    t = new Thread("t2");
    t->Fork(lock_and_increase, (int)args);
    lock_and_increase((int)args);
    return 1;
}

void i_am_not_holder(int arg) {
    Lock* lock = (Lock*) arg;
    printf("currentThread=%s, lock->is_held_by_current=%d\n", \
            currentThread->getName(), lock->isHeldByCurrentThread());
    ASSERT(lock->isHeldByCurrentThread() == false);
    currentThread->Yield();
}

int test_holder() {
    Lock *lock = new Lock("simplelock");
    int args = (int)lock;

    Thread* t = new Thread("not holder");
    lock->Acquire();
    t->Fork(i_am_not_holder, args);

    printf("currentThread=%s, lock->is_held_by_current=%d\n",
            currentThread->getName(), lock->isHeldByCurrentThread());
    ASSERT (lock->isHeldByCurrentThread() == true);
    for (int i = 0; i < 10; i++){
        currentThread->Yield();
    }
    return 1;
}

int test_delete() {      //to test the case that delete a lock that is held
    Lock *lock = new Lock("simplelock");
    lock->Acquire();    //the main thread held the lock
    printf("currentThread=%s, lock->is_held_by_current=%d\n",
            currentThread->getName(), lock->isHeldByCurrentThread());
    printf("currentThread=%s, is going to delete the lock\n", currentThread->getName());
    delete lock;    //the lock is deleted by the main thread which used to held it
    printf("Delete the lock which is held.\n");
    ASSERT(false);
    printf("Test failure.\n");
    return 1;
}
