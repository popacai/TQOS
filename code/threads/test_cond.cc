#include "test_cond.h"

void wait_thread(int _args) {
    int* args = (int*) _args;
	Lock* lock = (Lock*) args[0];
	Condition* cond = (Condition*) args[1];
	bool* resource_is_ready = (bool*) args[2];

	lock->Acquire();
	while (!*resource_is_ready) {
		printf("0\n");
        printf("Before wait: currentThread name = %s\n",currentThread->getName());
		cond->Wait(lock);
        printf("After wait: cuurentThread name = %s\n",currentThread->getName());
		printf("4\n");
	}
	lock->Release();
}

void broadcast_thread(int _args) {
    int* args = (int*) _args;
    Lock* lock = (Lock*) args[0];
    Condition* cond = (Condition*) args[1];
    bool* resource_is_ready = (bool*) args[2];
    
    lock->Acquire();
    *resource_is_ready = true;
    printf("Broadcast to all wait thread\n");
    cond->Broadcast(lock);
    lock->Release();
}

int test_broadcast_cond() {
    /*
    create 3 threads, two for wait, one for broadcase
    */    
    Lock* lock = new Lock("broadcastlock");
    Condition* cond = new Condition("broadcastcond");
    bool* resource_is_ready = new bool;
    *resource_is_ready = false;

    int* args = new int[3];
    args[0] = (int)lock;
    args[1] = (int)cond;
    args[2] = (int)resource_is_ready;

    Thread* t;
    t  = new Thread("wait1");
    t->Fork(wait_thread,(int)args);

    t  = new Thread("wait2");
    t->Fork(wait_thread,(int)args);
    
    t = new Thread("broadcast");
    t->Fork(broadcast_thread,(int)args);

    currentThread->Yield();
    return 1;
}

void signal_thread(int _args) {
	int* args = (int*) _args;
	Lock* lock = (Lock*) args[0];
	Condition* cond = (Condition*) args[1];
	bool* resource_is_ready = (bool*) args[2];

	lock->Acquire();
	printf("1\n");
	*resource_is_ready = true;
	cond->Signal(lock);
	printf("2\n");
	lock->Release();
}

int test_single_cond() {
	/*
	 * Create 3 threads. 2 threads wait and 1 thread signal.
	 * Expect one thread will be unpause and resume working.
	 * 4 always appear after 1 and 2
	 * 0 won't appear after 4 shows up
	 */
	Lock *lock = new Lock("conditionlock");
	Condition *cond = new Condition("condition");
	bool *resource_is_ready = new bool;
	*resource_is_ready = false;

	int* args = new int[3];
	args[0] = (int)lock;
	args[1] = (int)cond;
	args[2] = (int)resource_is_ready;

	Thread* t;
	t = new Thread("wait1");
	t->Fork(wait_thread, (int)args);

	t = new Thread("wait2");
	t->Fork(wait_thread, (int)args);

	t = new Thread("signal");
	t->Fork(signal_thread, (int)args);

	currentThread->Yield();
	return 1;
}
