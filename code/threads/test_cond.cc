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

void signaltest_thread(int _args) {
    int* args = (int*) _args;
    Lock* lock = (Lock*) args[0];
    Condition* cond = (Condition*) args[1];
    
//    lock->Acquire();
    cond->Signal(lock);
//    lock->Release();
}
 
int test_signal_cond() {   
    Lock* lock = new Lock("signallock");
    Condition* cond = new Condition("signalcond");

    int* args = new int[2];
    args[0] = (int)lock;
    args[1] = (int)cond;

    Thread* t;
    t  = new Thread("signal");
    t->Fork(signaltest_thread,(int)args);
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


void broadcast_nowaiter(int _args) {
    int* args = (int*) _args;
    Lock* lock = (Lock*) args[0];
    Condition* cond = (Condition*) args[1];
    
    lock->Acquire();
    ASSERT (cond->waitqueue_isempty());
    printf("Broadcast to all wait thread\n");
    cond->Broadcast(lock);
    lock->Release();
}

void signal_nowaiter(int _args) {
	int* args = (int*) _args;
	Lock* lock = (Lock*) args[0];
	Condition* cond = (Condition*) args[1];

	lock->Acquire();
	printf("1\n");
    ASSERT (cond->waitqueue_isempty());
	cond->Signal(lock);
	printf("2\n");
	lock->Release();
}

int test_nowaiter(int signal_or_broadcast) {
    /*
     * Create 2 threads, 1 thread signal or broadcasting when there is no waiters,
     * signal_or_broadcasting is 0 to test signal with no waiters
     * signal_or_broadcasting is 1 to test broadcasting with no waiters
     * Expect no-operation will be done,
     * Then the other thread wait should be blocked
     */
    Lock *lock = new Lock("conditionlock");
    Condition *cond = new Condition("condition");
    int testchoice = signal_or_broadcast;
    bool *resource_is_ready = new bool;
    *resource_is_ready = false;

    int* args = new int[3];
    args[0] = (int)lock;
    args[1] = (int)cond;
    args[2] = (int)resource_is_ready;

    Thread* t;
    
    if (testchoice==0) {
        t = new Thread("signal");
        t->Fork(signal_nowaiter, (int)args);
        printf("signaling\n");
    }else if (testchoice==1) {
        t = new Thread("broadcast");
        t->Fork(broadcast_nowaiter, (int)args);
        printf("broadcasting\n");
    }else
    printf("Test number is invalid.\n");

    currentThread->Yield();

    t = new Thread("wait");
    t->Fork(wait_thread, (int)args);
    
    currentThread->Yield();

    ASSERT (!cond->waitqueue_isempty());
    printf ("Success block wait.\n");
    return 1;
}
