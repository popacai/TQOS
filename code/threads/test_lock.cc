#include "test_lock.h"

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
        printf("currentThread=%s,%d\n", currentThread->getName(), *value);
        first_value_in_loop = *value;

	    currentThread->Yield();
        (*value)++;
        printf("currentThread=%s,%d\n", currentThread->getName(), *value);
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
