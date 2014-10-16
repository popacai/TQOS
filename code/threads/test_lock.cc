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

	for (int i = 0; i < 10; i++) {
        lock->Acquire();
        (*value)++;
        printf("currentThread=%s,%d\n", currentThread->getName(), *value);
	    currentThread->Yield();
        (*value)++;
        printf("currentThread=%s,%d\n", currentThread->getName(), *value);
        lock->Release();
	    currentThread->Yield();
        printf("currentThread=%s,%d\n", currentThread->getName(), *value);
        //ASSERT, value % 2 == 0
	}
}

int test_single_lock() {
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

