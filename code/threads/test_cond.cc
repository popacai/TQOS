#include "test_cond.h"

void signal_thread(int _args) {
	void* args = (void*) _args;
	Lock* lock = (Lock*) args[0];
	Condition* cond = (Condition*) args[1];
}

void wait_thread(int _args) {
    void* args = (void*) _args;
	Lock* lock = (Lock*) args[0];
	Condition* cond = (Condition*) args[1];

	lock->Acquire();
	while (true) {
		cond->Wait(lock);
	}
	lock->Release();
}

int test_single_cond() {
	/*
	 * Create 3 threads. 2 threads wait and 1 thread signal.
	 * Expect one thread will be unpause and resume working.
	 */
	Lock *lock = new Lock("conditionlock");
	Condition *cond = new Condition("condition");

	void* args = (void*) new int[2];
	args[0] = lock;
	args[1] = cond;

	Thread* t = new Thread("wait1");
	t->Fork(wait_thread, (int)args);
	currentThread->Yield();
	return 1;
}
